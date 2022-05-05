#include <ESP8266WiFi.h>

const char* ssid = "Dellink";
const char* password = "wtkh-daah-y8bj";

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);

  // Scanning Wi-Fis
  int numberOfNetworks = WiFi.scanNetworks();
  delay(4000);
  Serial.println();
  for(int i = 0; i < WiFi.scanNetworks(); i++){
      Serial.print("Network name: ");
      Serial.println(WiFi.SSID(i));
      Serial.println("-----------------------");
  }
  
  // Init Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("\nIP Address: ");
  Serial.print(WiFi.localIP());
}

void loop() {}
