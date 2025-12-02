// ==================================================
//   NODEMCU ESP8266 – PIR SENSOR CALIBRATION TOOL
// ==================================================

#define PIR_PIN 5   // D1

unsigned long motionStart = 0;
unsigned long motionEnd = 0;
bool motionActive = false;

void setup() {
  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT_PULLUP);

  Serial.println("========================================");
  Serial.println("      PIR SENSOR CALIBRATION MODE       ");
  Serial.println("========================================");
  Serial.println("Tips:");
  Serial.println("- Wait 30–60 seconds for PIR warm-up");
  Serial.println("- Move your hand at different distances");
  Serial.println("- Observe detection timing & stability");
  Serial.println("----------------------------------------");
}

void loop() {

  int pirValue = digitalRead(PIR_PIN);

  // Print raw digital value (0 or 1)
  Serial.print("PIR Value: ");
  Serial.println(pirValue);

  // Motion started
  if (pirValue == HIGH && !motionActive) {
    motionActive = true;
    motionStart = millis();
    Serial.print("  -> MOTION STARTED at ");
    Serial.print(motionStart / 1000.0);
    Serial.println(" sec");
  }

  // Motion ended
  if (pirValue == LOW && motionActive) {
    motionActive = false;
    motionEnd = millis();

    unsigned long duration = motionEnd - motionStart;

    Serial.print("  -> MOTION ENDED at ");
    Serial.print(motionEnd / 1000.0);
    Serial.println(" sec");

    Serial.print("     Motion Duration: ");
    Serial.print(duration / 1000.0);
    Serial.println(" sec");
  }

  delay(100);  // fast sampling
}
