#include <Servo.h>

#define R 2  // Red LED (Motion Detected)
#define Y 3  // Yellow LED (Closing Warning)
#define G 4  // Green LED (Gate Open)
#define sensor 5  // PIR Sensor Input

Servo servo;

void setup() {
  Serial.begin(9600);
  pinMode(R, OUTPUT);
  pinMode(Y, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(sensor, INPUT);
  
  servo.attach(9);
  servo.write(110);  // Initial position (Closed)
}

// Function to move servo smoothly
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

// Function to blink yellow LED before closing
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
    delay(500); // Double-check motion to avoid false triggers
    if (digitalRead(sensor) == HIGH) {  
      digitalWrite(R, HIGH);  // Motion detected
      smoothServoMove(110, 180, 30);  // Open gate smoothly
      digitalWrite(G, HIGH);  // Gate is open
      digitalWrite(R, LOW);
      delay(3000);  // Keep gate open for 2 seconds
      
      digitalWrite(G, LOW);
      blinkWarningLED();  // Blink yellow LED before closing
      digitalWrite(R, HIGH);
      smoothServoMove(180, 110, 30);  // Close gate smoothly
      digitalWrite(R, LOW);
    }
  }
  delay(1000);
}
