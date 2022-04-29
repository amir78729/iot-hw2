#include <ESP8266WiFi.h>

const char* ssid = "Dellink";
const char* password = "wtkh-daah-y8bj";

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);

  // Scanning Wi-Fis
  int numberOfNetworks = WiFi.scanNetworks();
 
  for(int i = 0; i < WiFi.scanNetworks(); i++){
      Serial.print("Network name: ");
      Serial.println(WiFi.SSID(i));
      Serial.println("-----------------------");
  }
  
  // Init Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(5000);
  }
  Serial.print("\nIP Address: ");
  Serial.print(WiFi.localIP());
}

void loop() {}
