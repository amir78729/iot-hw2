#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "webpage.h"
#include <Servo.h>

#define LED 2 // D4
#define SERVO 0 // D3
#define TRIGGER_PIN 4 // D4
#define ECHO_PIN 5 // D1
#define BUZZER_PIN 16 // D0 

#define SOUND_VELOCITY 0.034
bool flag = true;

ESP8266WebServer server(80);
const String DOOR_IS_CLOSED = "DOOR_IS_CLOSED";
const String DOOR_IS_OPEN = "DOOR_IS_OPEN";
const String OPENING_DOOR = "OPENING_DOOR";
const String CLOSING_DOOR = "CLOSING_DOOR";
const String OBJECT_DETECTED = "OBJECT_DETECTED";

const char *ssid = "Dellink";
const char *password = "wtkh-daah-y8bj";
bool is_connected = false;
String doorState = DOOR_IS_CLOSED;
String command = "wait";
int open_time = 0;
String alert_text = "";
int alert_time = 0;
int pos = 0;
bool is_object_detected;
Servo myservo;

void webpage() {
  server.send(200, "text/html", webpage_html);
}

void button() {
  server.send(200, "text/plain", "clicked");
  command = server.arg("state");
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

void alert() {
  server.send(200, "text/plain", alert_text);
}

void setup() {
  Serial.begin(9600);
  delay(500);
  pinMode(LED, OUTPUT);
  myservo.attach(SERVO);
  myservo.write(0);
  pinMode (TRIGGER_PIN, OUTPUT);  //ultrasonic sensor output pin
  pinMode (ECHO_PIN, INPUT); //ultrasonic sensor input pin
  pinMode(BUZZER_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }

  Serial.println();
  if (WiFi.status() == WL_CONNECTED){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    is_connected = true;
  }

  if (is_connected) {
    server.on("/", webpage);
    server.on("/button", button);
    server.on("/alert", alert);
    server.onNotFound(handle_NotFound);
    server.begin();
    Serial.println("Parking's server is up");
  } else {
    Serial.println("Connection failed");
  }
}

float get_distance_in_cm() {
  long duration;
  // Clears the TRIGGER_PIN
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  // Sets the TRIGGER_PIN on HIGH state for 10 micro seconds
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  // Reads the ECHO_PIN, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);
  // return the distance
  return duration * SOUND_VELOCITY / 2;
}

void open_door() {
  digitalWrite(LED, HIGH);
  while (pos < 180) {
    myservo.write(pos);
    delay(15);
    pos += 1;
  }
  open_time = millis();
  digitalWrite(LED, LOW);
  doorState = DOOR_IS_OPEN;
  alert_text = DOOR_IS_OPEN;
  command = "wait";
}
void close_door() {
  
  alert_text = CLOSING_DOOR;
  if (flag) {
    flag = false;
    delay(100);
    return;
  }
  flag = true;
  Serial.println("Closing the door...");
  for (pos = 180; pos >= 0; pos -= 1) {
    if (get_distance_in_cm() < 20) {
      Serial.println("Object was detected");
      alert_text = OBJECT_DETECTED;
      return;
    }
    myservo.write(pos);
    delay(15);
  }
  doorState = DOOR_IS_CLOSED;
  alert_text = DOOR_IS_CLOSED;
  command = "wait";
}


void loop() {
  server.handleClient();
  if (alert_text == OBJECT_DETECTED) {
    alert_text = OBJECT_DETECTED;
    if (flag) {
      digitalWrite(BUZZER_PIN, HIGH);
      flag = false;
      delay(500);
      digitalWrite(BUZZER_PIN, LOW);
      return;
    }
    flag = true;
    open_door();
  }
  if (command == "open" ) {
    if (doorState == DOOR_IS_CLOSED) {
      
      alert_text = OPENING_DOOR;
      if (flag) {
        flag = false;
        delay(100);
        return;
      }
      Serial.println("Opening the door...");
      flag = true;
      open_door();
      return;
    }
  }
  if (command == "close" && doorState == DOOR_IS_OPEN) {
    close_door();
    return;
  }
  if (command == "wait") {
    alert_text = doorState;
    if (doorState == DOOR_IS_OPEN) {
      if (millis() - open_time >= 20000) {
        close_door();
      }
    }

  }
}
