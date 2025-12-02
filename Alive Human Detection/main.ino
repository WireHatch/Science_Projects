// ===============================
// NODEMCU ESP8266 + PIR + 3 LED CHASER SYSTEM (1.5V PWM)
// ===============================

// ---- PIN DEFINITIONS (ESP8266 GPIO) ----
#define LED1_PIN  14   // D5
#define LED2_PIN  12   // D6
#define LED3_PIN  13   // D7
#define PIR_PIN    5   // D1 (PIR Sensor)

// ---- TIMER ----
unsigned long lastMotionTime = 0;
const unsigned long OFF_DELAY = 15000;  // 15 seconds after no motion

// ---- PWM VALUE FOR ~1.5V LED BRIGHTNESS ----
const int LED_POWER = 460;  // Around 1.5V equivalent PWM

// --------------------------------------
// LED CHASER WHEN MOTION IS DETECTED
// --------------------------------------
void chaserEffect() {
  analogWrite(LED1_PIN, LED_POWER);
  delay(100);

  analogWrite(LED2_PIN, LED_POWER);
  delay(100);

  analogWrite(LED3_PIN, LED_POWER);
  delay(100);
}

// --------------------------------------
// BLINK ALL LEDs BEFORE SHUTTING OFF
// --------------------------------------
void warningBlink() {
  for (int i = 0; i < 5; i++) {
    analogWrite(LED1_PIN, LED_POWER);
    analogWrite(LED2_PIN, LED_POWER);
    analogWrite(LED3_PIN, LED_POWER);
    delay(200);

    analogWrite(LED1_PIN, 0);
    analogWrite(LED2_PIN, 0);
    analogWrite(LED3_PIN, 0);
    delay(200);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  analogWriteRange(1023);  // default, ensures consistent PWM

  // Ensure OFF at start
  analogWrite(LED1_PIN, 0);
  analogWrite(LED2_PIN, 0);
  analogWrite(LED3_PIN, 0);

  Serial.println("System Ready - Waiting for Motion...");
}

void loop() {
  bool motion = digitalRead(PIR_PIN);

  if (motion == HIGH) {

    Serial.println("Motion Detected!");

    // LED Chaser Effect
    chaserEffect();

    // Keep LEDs ON with 1.5V brightness
    analogWrite(LED1_PIN, LED_POWER);
    analogWrite(LED2_PIN, LED_POWER);
    analogWrite(LED3_PIN, LED_POWER);

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
      analogWrite(LED1_PIN, 0);
      analogWrite(LED2_PIN, 0);
      analogWrite(LED3_PIN, 0);

      Serial.println("No Motion - LEDs OFF");
    }
  }

  delay(150);
}
