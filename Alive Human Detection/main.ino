// ===============================
// NODEMCU ESP8266 + PIR + Light + Fan
// ===============================

// ---- PIN DEFINITIONS (ESP8266 GPIO) ----
#define LIGHT_PIN   14   // D5
#define FAN_PIN     12   // D6
#define PIR_PIN     5    // D1 (PIR Sensor)

// ---- TIMER ----
unsigned long lastMotionTime = 0;
const unsigned long OFF_DELAY = 15000;  // 15 seconds after no motion

void setup() {
  Serial.begin(9600);

  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  // Ensure OFF at start
  digitalWrite(LIGHT_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);

  Serial.println("System Ready - Waiting for Motion...");
}

void loop() {
  bool motion = digitalRead(PIR_PIN);

  if (motion == HIGH) {
    Serial.println("Motion Detected!");

    // Turn ON devices
    digitalWrite(LIGHT_PIN, HIGH);
    digitalWrite(FAN_PIN, HIGH);

    // Update last motion time
    lastMotionTime = millis();
  }
  else {
    // If no motion for OFF_DELAY time → turn Off
    if (millis() - lastMotionTime >= OFF_DELAY) {
      digitalWrite(LIGHT_PIN, LOW);
      digitalWrite(FAN_PIN, LOW);
      Serial.println("No Motion - Devices OFF");
    }
  }

  delay(200);
}
