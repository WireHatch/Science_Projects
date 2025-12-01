// ===============================
// UNIVERSAL SERVO CALIBRATION CODE
// Works on ESP32 and ESP8266
// ===============================

#include <ESP32Servo.h>   // Safe to include: works also on ESP8266 using Servo.h internally
Servo servo;

#define SERVO_PIN 13   // Change according to your wiring

void setup() {
  Serial.begin(9600);
  delay(500);

  Serial.println();
  Serial.println("=== SERVO CALIBRATION TOOL ===");
  Serial.println("Enter angle between 0 – 180");
  Serial.println("Type values slowly (e.g., 0, 30, 90, 180)");

#if defined(ESP32)
  // Allocate timers for ESP32 Servo Library
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  servo.setPeriodHertz(50);       // Standard servo frequency 50Hz
  servo.attach(SERVO_PIN, 500, 2400);  // Min/Max pulse width for fine calibration
#else
  // ESP8266 uses normal attach
  servo.attach(SERVO_PIN);        // Uses default 544–2400 μs
#endif
}

void loop() {
  if (Serial.available()) {
    int angle = Serial.parseInt();  // Read number

    if (angle >= 0 && angle <= 180) {
      servo.write(angle);
      Serial.print("Moved to angle: ");
      Serial.println(angle);
    } else {
      Serial.println("Invalid! Enter a value between 0–180.");
    }
  }
}
