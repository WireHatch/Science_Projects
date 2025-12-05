// ===============================
// NODEMCU ESP8266 + SERVO + RAIN SENSOR
// ===============================

#include <Servo.h>
#include <Wire.h>

Servo servo;

// ---- PIN DEFINITIONS (ESP8266 GPIO) ----
#define RAIN_SENSOR  D3   // D3 (Rain sensor digital output)
#define SERVO_PIN    D5   // D5

// ---- SERVO POSITIONS ----
int close_pos = 90;     // Lid/roof closed
int open_pos  = 0;    // Lid/roof open (change if needed)

void setup() {
  Serial.begin(9600);

  pinMode(RAIN_SENSOR, INPUT);

  // ---- SERVO INIT ----  
  servo.attach(SERVO_PIN, 500, 2400);  
  servo.write(close_pos);   // Start closed

  Serial.println("Rain Sensor + Servo System Started");
}

void loop() {

  bool rainDetected = digitalRead(RAIN_SENSOR);  
  // IMPORTANT:
  // Many rain modules output:
  //     0 = Rain detected
  //     1 = No rain

  Serial.print("Rain Sensor State: ");
  Serial.println(rainDetected);

  if (rainDetected == 1) {
    // Rain present
    Serial.println("Rain detected → Opening roof/cover");
    servo.write(open_pos);
  }
  else {
    // No rain
    Serial.println("No rain → Closing roof/cover");
    servo.write(close_pos);
  }

  delay(300);
}
