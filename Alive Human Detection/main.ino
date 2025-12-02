// ===============================
// NODEMCU ESP8266 + PIR + 3 LED CHASER SYSTEM
// ===============================

// ---- PIN DEFINITIONS (ESP8266 GPIO) ----
#define LED1_PIN  14   // D5
#define LED2_PIN  12   // D6
#define LED3_PIN  13   // D7
#define PIR_PIN    5   // D1 (PIR Sensor)

// ---- TIMER ----
unsigned long lastMotionTime = 0;
const unsigned long OFF_DELAY = 15000;  // 15 seconds after no motion

// --------------------------------------
// LED CHASER WHEN MOTION IS DETECTED
// --------------------------------------
void chaserEffect() {
  digitalWrite(LED1_PIN, HIGH);  
  delay(100);
  digitalWrite(LED2_PIN, HIGH);
  delay(100);
  digitalWrite(LED3_PIN, HIGH);
  delay(100);
}

// --------------------------------------
// BLINK ALL LEDs BEFORE SHUTTING OFF
// --------------------------------------
void warningBlink() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED3_PIN, HIGH);
    delay(200);

    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    delay(200);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  // Ensure OFF at start
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  Serial.println("System Ready - Waiting for Motion...");
}

void loop() {
  bool motion = digitalRead(PIR_PIN);

  if (motion == HIGH) {

    Serial.println("Motion Detected!");

    // LED Chaser Effect
    chaserEffect();

    // Keep LEDs ON
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED3_PIN, HIGH);

    // Reset timer
    lastMotionTime = millis();
  }
  else {
    // Check auto OFF condition
    if (millis() - lastMotionTime >= OFF_DELAY) {
      
      Serial.println("Preparing to turn OFF...");

      // Warning blink before shutting down
      warningBlink();

      // Turn all LEDs OFF
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, LOW);
      digitalWrite(LED3_PIN, LOW);

      Serial.println("No Motion - LEDs OFF");
    }
  }

  delay(150);
}
