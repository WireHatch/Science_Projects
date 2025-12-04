// ===============================================
// UNIVERSAL ESP32 + ESP8266
// PIR + 3 LED CHASER SYSTEM (1.5V PWM)
// ===============================================
// #if defined(ESP32)
  #include <Arduino.h>
// #endif
int pirState = 0;

// // --------- PIN DEFINITIONS ----------
// #if defined(ESP8266)
#define LED1_PIN  14   // D5
#define LED2_PIN  12   // D6
#define LED3_PIN  13   // D7
#define PIR_PIN    8   // D1
// #else
//   // ESP32 GPIO
//   #define LED1_PIN  26
//   #define LED2_PIN  27
//   #define LED3_PIN  25
//   #define PIR_PIN   12
// #endif

// -------- TIMER & STATE --------
unsigned long lastMotionTime = 0;
const unsigned long OFF_DELAY = 5000;
bool ledsOn = false;
bool chaserPlayed = false;

// -------- PWM BRIGHTNESS (1.5V approx.) --------
const int LED_POWER = 120;

// ============ ESP32 PWM SETUP ============
// #if defined(ESP32)
//   const int PWM_FREQ = 5000;
//   const int PWM_RES  = 8;   // 0–255
//   const int CH1 = 0;
//   const int CH2 = 1;
//   const int CH3 = 2;
// #endif

// ============ UNIFIED WRITE FUNCTION ============
void writeLED(int pin, int value) {
// #if defined(ESP32)
//   ledcWrite(pin, value);
// #else
analogWrite(pin, value);
// #endif
}

// ============ LED CHASER EFFECT ============
void chaserEffect() {
Serial.println("Running chaser animation...");

// LED1 lights up
writeLED(LED1_PIN, LED_POWER);
writeLED(LED2_PIN, 0);
writeLED(LED3_PIN, 0);
delay(150);

// LED2 lights up
writeLED(LED1_PIN, LED_POWER);
writeLED(LED2_PIN, LED_POWER);
writeLED(LED3_PIN, 0);
delay(150);

// LED3 lights up (all on now)
writeLED(LED1_PIN, LED_POWER);
writeLED(LED2_PIN, LED_POWER);
writeLED(LED3_PIN, LED_POWER);
delay(150);
}

// ============ WARNING BLINK (ACCELERATING) ============
void warningBlink() {
Serial.println("Warning: No motion - blinking...");

int delayTime = 300;

for (int i = 0; i < 8; i++) {
  // All LEDs blink together
  writeLED(LED1_PIN, LED_POWER);
  writeLED(LED2_PIN, LED_POWER);
  writeLED(LED3_PIN, LED_POWER);
  delay(delayTime);
  
  writeLED(LED1_PIN, 0);
  writeLED(LED2_PIN, 0);
  writeLED(LED3_PIN, 0);
  delay(delayTime);
  
  // Gradually reduce delay (speed up blinking)
  delayTime -= 30;
  if (delayTime < 50) delayTime = 50;
}
}

// ============ TURN OFF ALL LEDS ============
void allLedsOff() {
writeLED(LED1_PIN, 0);
writeLED(LED2_PIN, 0);
writeLED(LED3_PIN, 0);
}

// ================= SETUP =================
void setup() {
Serial.begin(9600);

// #if defined(ESP32)
//   // Initialize PWM channels
//   ledcAttachChannel(LED1_PIN, PWM_FREQ, PWM_RES, CH1);
//   ledcAttachChannel(LED2_PIN, PWM_FREQ, PWM_RES, CH2);
//   ledcAttachChannel(LED3_PIN, PWM_FREQ, PWM_RES, CH3);
// #else
pinMode(LED1_PIN, OUTPUT);
pinMode(LED2_PIN, OUTPUT);
pinMode(LED3_PIN, OUTPUT);
// #endif

pinMode(PIR_PIN, INPUT);

// Ensure LEDs off at boot
allLedsOff();

Serial.println("System Ready - Waiting for Motion...");
}

void loop() {
  bool motion = digitalRead(PIR_PIN);

  // --- Detect RISING EDGE (Motion starts) ---
  if (motion == HIGH && pirState == LOW) {
    pirState = HIGH;
    Serial.println("Motion detected!");

    // Play chaser animation when motion starts
    chaserEffect();

    ledsOn = true;
    chaserPlayed = true;

    // Keep LEDs ON
    writeLED(LED1_PIN, LED_POWER);
    writeLED(LED2_PIN, LED_POWER);
    writeLED(LED3_PIN, LED_POWER);

    lastMotionTime = millis();
  }

  // --- Detect CONTINUOUS MOTION ---
  else if (motion == HIGH && pirState == HIGH) {
    // Maintain LEDs ON
    writeLED(LED1_PIN, LED_POWER);
    writeLED(LED2_PIN, LED_POWER);
    writeLED(LED3_PIN, LED_POWER);

    // Keep resetting motion timer
    lastMotionTime = millis();
  }

  // --- Detect FALLING EDGE (Motion ended) ---
  else if (motion == LOW && pirState == HIGH) {
    pirState = LOW;
    Serial.println("Motion ended!");
    // Now we wait for OFF_DELAY before turning LEDs off
  }

  // --- No motion and state is OFF ---
  else if (motion == LOW && ledsOn == true) {
    if (millis() - lastMotionTime >= OFF_DELAY) {
      Serial.println("No motion for 5 seconds...");

      warningBlink();
      allLedsOff();

      Serial.println("LEDs OFF - Waiting for next motion...");

      ledsOn = false;
      chaserPlayed = false;
    }
  }

  delay(50);
}

