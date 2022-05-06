#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "homePage.h"
#include "itemPage.h"
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
bool flag = true;

// tags information
const String tags[] = {"336ff31a", "4c661249"};
bool is_tag_valid[] = {true, false};

const int SERVO_PIN = 4; // assiging servo pin: D2
const int LED_PIN = 5;  // assiging LED pin: D1
const int BUZZER_PIN = 0; // assiging buzzer pin: D3
const int SS_PIN = 15; // assiging RFID SS pin: D8
const int RST_PIN = 16; // assiging RFID RST pin: D0
// initating RFID
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
// Init array that will store new NUID
byte nuidPICC[4];
Servo myServo;

ESP8266WebServer server(80);
// wifi credentials
const char *ssid     = "ssid";
const char *password = "password";
uint8_t retries = 0;
bool is_connected = false;

//machine statuses
const String INVALID = "INVALID";
const String WAITING_FOR_TAG = "WAITING_FOR_TAG";
const String AUTHENTICATING = "AUTHENTICATING";
const String AUTHENTICATION_FAILED = "AUTHENTICATION_FAILED";
const String CHECKING_WALLET = "CHECKING_WALLET";
const String NOT_ENOUGH_MONEY = "NOT_ENOUGH_MONEY";
const String OPEN_DOOR = "OPENNING_DOOR";
const String TAKING = "TAKING";
const String DONE = "DONE";
String order_status = "-";

// authentication data
long authentication_started = -1;
const int price[] = {7000, 5000, 4000, 6000};
int ordered_item = -1;
int wallet = 20000;

void webpage() {
  server.send(200, "text/html", homePage_html);
  ordered_item = -1;
  order_status = INVALID;
}

void handle_order() {
  server.send(200, "text/html", itemPage_html);
  String order_id = server.arg("item");
  order_id.trim();
  Serial.print("Order for item: ");
  Serial.println(order_id);
  ordered_item = order_id.toInt() - 1; 
  Serial.println(ordered_item);
  Serial.println(price[ordered_item]);
  order_status = WAITING_FOR_TAG;
}

void return_status() {
  server.send(200, "text/plain", order_status);
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(9600);
  delay(500);
  myServo.attach(SERVO_PIN);

  // set up RFID
  SPI.begin();
  rfid.PCD_Init(); // Init MFRC522
  Serial.println();
  Serial.print(F("Reader :"));
  rfid.PCD_DumpVersionToSerial();
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  // set up Pin
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // connecting to wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    Serial.print(".");
    retries++;
    delay(1000);
  }
  Serial.println();
  if (retries == 20) {
    Serial.print("Unable to Connect to ");
    Serial.println(ssid);
  }
  // getting IP
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Successfully connected to ");
    Serial.println(ssid);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    is_connected = true;
  }
  // setting up the server
  if (is_connected) {
    server.on("/", webpage);
    server.on("/order", handle_order);
    server.on("/status", return_status);
    server.onNotFound(handle_NotFound);
    server.begin();
    Serial.println("Vending machine's server is up");
  } else {
    Serial.println("Unable to set up the Server");
  }
  Serial.println("");
}

void loop() {
  if (!is_connected) {
    return;
  }
  server.handleClient();
  if (order_status == WAITING_FOR_TAG || order_status == AUTHENTICATING) {
    authenticate();
    return;
  }
  if (order_status == CHECKING_WALLET) {
    check_wallet();
    return;
  }
  if (order_status == OPEN_DOOR) {
    open_the_door();
    return;
  }
  if ( order_status == TAKING ) {
    notify_user();
    return;
  }
  if (order_status == DONE) {
    close_door();
    ordered_item = -1;
    return;
  }
}

void authenticate() {
  // if no card was detected in 60 seconds...
  if (order_status == AUTHENTICATING && millis() - authentication_started >= 60000) {
    order_status = AUTHENTICATION_FAILED;
    if (flag) {
      flag = false;
      delay(100);
      return;
    }
    flag = true;
    Serial.println("Authenticating timeout...");
    return;

    // when someone has pressed submit button after selecting a snack...
  } else if (order_status == WAITING_FOR_TAG) {
    authentication_started = millis();
    order_status = AUTHENTICATING;
    if (flag) {
      flag = false;
      delay(100);
      return;
    }
    flag = true;
  }

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent()) return;
  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial()) return;

  // Authorization...
  String tag = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    tag += String(rfid.uid.uidByte[i], HEX);
  }
  Serial.print("Tag: ");
  Serial.println(tag);
  for (int i = 0; i < 2; i += 1) {
    if (tag == tags[i]) {
      if (is_tag_valid[i]) {
        // a CE student was detected!
        order_status = CHECKING_WALLET;
        if (flag) {
          flag = false;
          delay(100);
          return;
        }
        flag = true;
        Serial.println("Authenticated");
        delay(500);
      } else {
        order_status = AUTHENTICATION_FAILED;
        if (flag) {
          flag = false;
          delay(100);
          return;
        }
        flag = true;
        Serial.println("Authentication Failed");
      }
    }
  }
  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

void buzz() {
  tone(BUZZER_PIN, 1000);
  delay(1000);
  noTone(BUZZER_PIN);
}

void check_wallet() {
  if ( ordered_item < 0 || ordered_item > 3) {
    Serial.println("No item is selected, ERROR");
  } else {
    // if order was valid...checking wallet...
    if ( wallet - price[ordered_item] < 0 ) {
      order_status = NOT_ENOUGH_MONEY;
      if (flag) {
        flag = false;
        buzz();
        return;
      }
      flag = true;
    } else {
      wallet -= price[ordered_item];
      Serial.print("Remained Balance:");
      Serial.println(wallet);
      order_status = OPEN_DOOR;
      if (flag) {
        flag = false;
        delay(100);
        return;
      } else {
        delay(200);
      }
      flag = true;
    }
  }
}

void open_the_door() {
  int pos;
  for (pos = 0 ; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  order_status = TAKING;
  if (flag) {
    flag = false;
    delay(100);
    return;
  }
  flag = true;
}

void notify_user() {
  digitalWrite(LED_PIN, HIGH);
  delay(6000);
  digitalWrite(LED_PIN, LOW);
  order_status = DONE;
  if (flag) {
    flag = false;
    delay(100);
    return;
  }
  flag = true;
}

void close_door() {
  if ( myServo.read() == 0 ) { // door already closed
    return;
  }
  int pos;
  for (pos = 180 ; pos >= 0; pos -= 1) { // goes from 0 degrees to 180 degrees
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
