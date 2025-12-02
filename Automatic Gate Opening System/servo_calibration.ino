#include <Servo.h>

#define SERVO_PIN 2   // D4 on NodeMCU

Servo servo;

void setup() {
  Serial.begin(9600);

  // MG90S recommended pulse width range
  servo.attach(SERVO_PIN, 500, 2400);

  Serial.println("======================================");
  Serial.println("      MG90S SERVO CALIBRATION");
  Serial.println("======================================");
  Serial.println("Type an angle (0–180) and press ENTER");
  Serial.println("Example: 90");
  Serial.println("======================================");
}

String inputString = "";
bool newData = false;

void loop() {

  // ----------- Read Serial ---------------
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      newData = true;
      break;
    } else {
      inputString += c;
    }
  }

  // ---------- Process User Input ----------
  if (newData) {
    int angle = inputString.toInt();
    inputString = "";
    newData = false;

    if (angle >= 0 && angle <= 180) {
      servo.write(angle);
      Serial.print("MG90S moved to: ");
      Serial.println(angle);
    } else {
      Serial.println("Invalid angle! Enter 0–180 only.");
    }

    Serial.println("--------------------------------------");
    Serial.println("Enter next angle:");
  }
}
