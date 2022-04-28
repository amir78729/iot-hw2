#include <ESP8266WiFi.h>

IPAddress local_IP(10, 10, 10, 1);
IPAddress gateway(10, 10, 10, 1);
IPAddress subnet(255, 255, 255, 0);

void setup()
{
  Serial.begin(9600);
  delay(3000);

  WiFi.softAP("ESP8266-AP");

  Serial.print("IP address (Before Cofig): ");
  Serial.println(WiFi.softAPIP());

  Serial.print("Setting configuration ... ");
  WiFi.softAPConfig(local_IP, gateway, subnet);
  
  Serial.print("IP address (After Config): ");
  Serial.println(WiFi.softAPIP());
}

void loop() {}
