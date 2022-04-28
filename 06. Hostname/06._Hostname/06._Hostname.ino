#include <ESP8266WiFi.h>

String newHostname = "MyNodeMCU";

void setup() {
  Serial.begin(9600);
  delay(5000);
  WiFi.mode(WIFI_STA);

  //Get Current Hostname
  Serial.printf("Default hostname: %s\n", WiFi.hostname().c_str());

  //Set new hostname
  WiFi.hostname(newHostname.c_str());

  //Get Current Hostname
  Serial.printf("New hostname: %s\n", WiFi.hostname().c_str());
}

void loop() {
  // put your main code here, to run repeatedly:
}
