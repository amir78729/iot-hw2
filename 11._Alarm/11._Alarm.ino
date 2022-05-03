#include <NTPClient.h>
#include <WiFiUdp.h>
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "webpage.h"

// defining buzzer pin
//#define BUZZER_PIN 9; // assiging buzzer pin: SD2

long int input_time;
String volume;
String state;

const int SNOOZE_DURATION = 5000;

// global variables
int timer_millies = -1;
bool snooze_alarm = false;
bool stop_alarm = false;

// informations and instances for connecting to the internet

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
ESP8266WebServer server(80);

const char *ssid     = "Dellink";
const char *password = "wtkh-daah-y8bj";


void alarm_info() {
  server.send(200, "text/plain", "clicked");
  String tmp = server.arg("time");

  Serial.println(tmp);
  tmp.replace("000", "");
  Serial.println(tmp);
  input_time = tmp.toInt();
  Serial.println(input_time);
  volume = server.arg("volume");
  state = server.arg("state");

  //  Serial.println(server.arg("time").replace("000", ""));
  //  Serial.println(server.arg("time").replace("000", "").toInt());
  //  Serial.println(server.arg("time").replace("000", "").toInt() / 1000);
  //  Serial.println(floor(server.arg("time").replace("000", "").toInt() / 1000));

  Serial.println("----------------------------------------------------------------------------------");
  Serial.print("input_time: ");
  Serial.println(input_time);
  Serial.print("volume: ");
  Serial.println(volume);
  Serial.print("state: ");
  Serial.println(state);
  if (state == "snooz") {
    snooze_alarm = true;
  }
  Serial.println("----------------------------------------------------------------------------------");

}
void webpage() {
  server.send(200, "text/html", webpage_html);
}
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

void setup() {
  //  pinMode(BUZZER_PIN, OUTPUT);
  //  digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();

  // getting IP
  bool is_connected = false;
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
    server.on("/alarm_info", alarm_info);
    server.onNotFound(handle_NotFound);
    server.begin();
    Serial.println("Server is up");
  } else {
    Serial.println("Unable to set up the Server");
  }
  Serial.println("");
  timeClient.begin();
  timeClient.setTimeOffset(16200); // GMT+4:30 (4.5*3600)
  timeClient.update();
  Serial.println();
  print_current_time();
  Serial.println("Connected to the Internet!");
}
int snooz_start_time = -1;

void loop() {
  server.handleClient();
  timeClient.update();
    Serial.println(input_time);
    Serial.println((timeClient.getEpochTime()));
    Serial.println();
  //  if (BUZZER_PIN, (timeClient.getEpochTime() >= input_time && !snooze_alarm && !stop_alarm)) {
  //    buzz();
  //  }
  if (state == "start") {
    if (timeClient.getEpochTime() >= input_time) {
      Serial.println("timeClient.getEpochTime() >= input_time");
    }
    if (snooze_alarm) {
      if (snooz_start_time == -1) {
        // initaiting snoozing
        Serial.println("snooz start");
        snooz_start_time = millis();
      } else {
        if (millis() - snooz_start_time >= 5000) {
          snooz_start_time = -1;
          snooze_alarm = false;
          Serial.println("end of snooz");
        }
      }
      //      snooze_alarm();
      //      buzz();
    }
    //    else if (stop_alarm) {
    //      //      stop_alarm();
    //    }
  }
}


//void buzz() {
//  digitalWrite(BUZZER_PIN, HIGH); //analogWrite
//}

void print_current_time() {
  Serial.println("----------------------------------------------------------------------------------");
  Serial.print("[");
  Serial.print(timeClient.getFormattedTime());
  Serial.print("] ");
}

//void snooze_alarm() {
//  Serial.print("snoozing...(");
//  Serial.print(SNOOZE_DURATION - (millis() - timer_millies));
//  Serial.println("ms remaining)");
//}

//void stop_alarm() {
//  digitalWrite(BUZZER_PIN, LOW);
//}
