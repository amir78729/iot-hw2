#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

// defining buzzer pin
const int BUZZER_PIN = 9; // assiging buzzer pin: SD2
const int[8] INPUT_TIME;

const int SNOOZE_DURATION = 5000;

// global variables
int timer_millies = -1;
bool snooze_alarm = false;
bool stop_alarm = false;

// informations and instances for connecting to the internet
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
const char *ssid     = "<SSID>";
const char *password = "<PASSWORD>";

// function prototypes
void print_current_time(void);
void buzz(void);
void snooze_alarm(void);
void stop_alarm(void);

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(9600);
  WiFi.begin(ssid, password); // connecting to the internet for getting the time
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  timeClient.begin();
  timeClient.setTimeOffset(16200); // GMT+4:30 (4.5*3600)
  timeClient.update();
  Serial.println();
  print_current_time();
  Serial.println("Connected to the Internet!");
}

void loop() {
  timeClient.update();
  if (!stop_alarm) {
    if (timeClient.getEpochTime() >= INPUT_TIME)
    {
      buzz();
    }
    if (snooze_alarm) {
      snooze_alarm();
      buzz();
    }
    else if (stop_alarm) {
      stop_alarm();
    }
  }
}


void buzz() {
  digitalWrite(BUZZER_PIN, HIGH); //analogWrite
}

void print_current_time() {
  Serial.println("----------------------------------------------------------------------------------");
  Serial.print("[");
  Serial.print(timeClient.getFormattedTime());
  Serial.print("] ");
}

void snooze_alarm() {
  Serial.print("snoozing...(");
  Serial.print(SNOOZE_DURATION - (millis() - timer_millies));
  Serial.println("ms remaining)");
}

void stop_alarm() {
  digitalWrite(BUZZER_PIN, LOW);
}
