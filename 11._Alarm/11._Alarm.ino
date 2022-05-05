#include <NTPClient.h>
#include <WiFiUdp.h>
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "webpage.h"

// defining buzzer pin
#define BUZZER_PIN 5 // assiging buzzer pin: D1

long int input_time;
String volume;
String state;
bool alarm_was_set_in_the_past = false;

const int SNOOZE_DURATION = 5000;

// global variables
int timer_millies = -1;
bool snooze_alarm = false;
bool stop_alarm = false;

// informations and instances for connecting to the internet

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 16200);
ESP8266WebServer server(80);

const char *ssid     = "Dellink";
const char *password = "wtkh-daah-y8bj";

void update_time_func() {
  // when the user changes the time from the wabpage...
  server.send(200, "text/plain", "clicked");
  String tmp = server.arg("time");
  tmp.replace("000", "");
  input_time = tmp.toInt();
  print_current_time();
  Serial.print("input_time: ");
  Serial.println(input_time);
  if (timeClient.getEpochTime() - 16200 >= input_time) {
    alarm_was_set_in_the_past = true;
  } else {
    alarm_was_set_in_the_past = false;
  }
  stop_alarm = false;
  snooze_alarm = false;
  Serial.println("----------------------------------------------------------------------------------");
}

void update_volume_func() {
  // when the user changes the volume from the wabpage...
  server.send(200, "text/plain", "clicked");
  volume = server.arg("volume");
  print_current_time();
  Serial.print("volume: ");
  Serial.println(volume);
  Serial.println("----------------------------------------------------------------------------------");
}
void snooze_alarm_func() {
  // when the user clicks on snooze button...
  server.send(200, "text/plain", "clicked");
  print_current_time();
  Serial.println("User Pressed the Snooze Button");
  Serial.println("----------------------------------------------------------------------------------");
  stop_alarm = false;
  snooze_alarm = true;
}
void stop_alarm_func() {
  // when the user clicks on stop button...
  server.send(200, "text/plain", "clicked");
  stop_alarm = true;
  snooze_alarm = false;
  print_current_time();
  Serial.println("User Pressed the Stop Button");
  Serial.println("----------------------------------------------------------------------------------");
}
void webpage() {
  server.send(200, "text/html", webpage_html);
}
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

void setup() {
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
    server.on("/update_time", update_time_func);
    server.on("/update_volume", update_volume_func);
    server.on("/snooze_alarm", snooze_alarm_func);
    server.on("/stop_alarm", stop_alarm_func);
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
bool active_alarm = false;
int i = 0;
void loop() {
  server.handleClient();
  timeClient.update();
  if (active_alarm && !snooze_alarm && !stop_alarm && !alarm_was_set_in_the_past) {
    buzz();
    i++;
    if (i > 82) {
      Serial.println("!");
      i = 0;
    } else {
      Serial.print("!");
    }
  } else {
    analogWrite(BUZZER_PIN, 0);
  }

  if (timeClient.getEpochTime() - 16200 >= input_time) {
    active_alarm = true;
  } else {
    active_alarm = false;
  }

  if (snooze_alarm) {
    if (snooz_start_time == -1) {
      // initaiting snoozing
      Serial.println("snooze started (5 seconds)");
      snooz_start_time = millis();
    } else {
      if (millis() - snooz_start_time >= 5000) {
        snooz_start_time = -1;
        snooze_alarm = false;
        Serial.println("end of snooze");
      }
    }
  }
}


void buzz() {
  analogWrite(BUZZER_PIN, map(volume.toInt(), 0, 10, 0, 255));
}

void print_current_time() {
  Serial.println("\n----------------------------------------------------------------------------------");
  Serial.print("[");
  Serial.print(timeClient.getFormattedTime());
  Serial.print("] ");
}
