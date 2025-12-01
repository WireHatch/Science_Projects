// =====================================
// NODEMCU (ESP8266) WORKING VERSION
// =====================================

#include <ESP8266Servo.h>     // Correct library for NodeMCU/ESP8266
Servo servo;

// ---- PIN DEFINITIONS (NODEMCU) ----
#define R  D5     // GPIO14 - Red LED
#define Y  D6     // GPIO12 - Yellow LED
#define G  D7     // GPIO13 - Green LED
#define sensor D1 // GPIO5  - PIR Sensor
#define SERVO_PIN D4 // GPIO2 - Servo Pin

void setup() {
  Serial.begin(9600);

  pinMode(R, OUTPUT);
  pinMode(Y, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(sensor, INPUT);

  // Attach servo normally (ESP8266 does not need PWM timers)
  servo.attach(SERVO_PIN, 500, 2400);

  servo.write(110);  // Gate closed position
}

// ----- Smooth Servo Movement -----
void smoothServoMove(int start, int end, int stepDelay) {
  if (start < end) {
    for (int pos = start; pos <= end; pos++) {
      servo.write(pos);
      delay(stepDelay);
    }
  } else {
    for (int pos = start; pos >= end; pos--) {
      servo.write(pos);
      delay(stepDelay);
    }
  }
}

// ----- Yellow LED Warning -----
void blinkWarningLED() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(Y, HIGH);
    delay(300);
    digitalWrite(Y, LOW);
    delay(300);
  }
}

void loop() {
  bool value = digitalRead(sensor);
  Serial.println(value);

  if (value == HIGH) {
    delay(500);

    if (digitalRead(sensor) == HIGH) {  
      digitalWrite(R, HIGH);

      smoothServoMove(110, 180, 30);
      digitalWrite(G, HIGH);
      digitalWrite(R, LOW);

      delay(3000);

      digitalWrite(G, LOW);
      blinkWarningLED();

      digitalWrite(R, HIGH);
      smoothServoMove(180, 110, 30);
      digitalWrite(R, LOW);
    }
  }

  delay(300);
}
