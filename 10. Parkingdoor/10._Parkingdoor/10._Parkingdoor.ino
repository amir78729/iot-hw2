#include <Servo.h>

Servo myservo;  // create servo object to control a servo
const int TRIGGER_PIN = 12; // assiging trigger pin for ultrasonic sensor: D6
const int ECHO_PIN = 14; // assiging echo pin for ultrasonic sensor: D5
const int SERVO_PIN = 0; // assiging servo pin: D3
const int LED_PIN = 2; // assiging servo pin: D4

const int TIMER_DURATION = 5000;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034

long duration;
float distance_in_cm;
bool is_target_detected = false;
bool is_open = false;
int timer_millies = -1;
int pos = 0;

void setup() {
  myservo.attach(SERVO_PIN);  // attaching servo to the servo object
  Serial.begin(9600);
  pinMode(TRIGGER_PIN, OUTPUT); // Sets the TRIGGER_PIN as an Output
  pinMode(ECHO_PIN, INPUT); // Sets the ECHO_PIN as an Input
  pinMode(LED_PIN, OUTPUT); // Sets the ECHO_PIN as an Input
  myservo.write(pos);
}

void loop() {
  distance_in_cm = get_distance_in_cm();
  
  if (distance_in_cm > 10 ) { // no object was detected closer than 10cm
    is_target_detected = false;
    if (is_open) {
      close_the_door();
    }
    
  } else { // an object was detected closer than 10cm
    
    if (!is_target_detected) { // the object was appeared recently
      
      Serial.println("the object was appeared recently");
      is_target_detected = true;
      timer_millies = millis();
      
    } else {
      
      if (millis() > timer_millies + TIMER_DURATION) { // the timer has been finished and we can open the door
        
        if (!is_open) {
          open_the_door();
        }
        
      } else { // we should wait for timer...
        wait_for_timer();
      }
    }
  }
  digitalWrite(LED_PIN, is_target_detected ? HIGH : LOW);
}

float get_distance_in_cm() {
  
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

void close_the_door() {
  Serial.println("closing the door");
  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
  }
  is_open = false;
}

void open_the_door() {
  Serial.println("the timer has been finished and we can open the door");
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    myservo.write(pos);
  }
  is_open = true;
}

void wait_for_timer() {
  Serial.print("waiting for timer...(");
  Serial.print(TIMER_DURATION - (millis() - timer_millies));
  Serial.println("ms remaining)");
}