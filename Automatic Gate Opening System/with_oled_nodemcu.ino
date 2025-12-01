// ===============================
// NODEMCU ESP8266 + OLED DISPLAY
// ===============================

#include <ESP8266WiFi.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Servo servo;

// ---- OLED SETTINGS ----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---- PIN DEFINITIONS (ESP8266 GPIO) ----
#define R       14    // D5
#define Y       12    // D6
#define G       13    // D7
#define sensor  5     // D1 = PIR sensor
#define SERVO_PIN  2  // D4 servo pin

// ---- OLED TEXT FUNCTION ----
void showMessage(String msg) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Count words
  int wordCount = 1;
  for (int i = 0; i < msg.length(); i++) {
    if (msg[i] == ' ') wordCount++;
  }

  // Split words
  String words[wordCount];
  int index = 0;
  String temp = "";

  for (int i = 0; i < msg.length(); i++) {
    if (msg[i] == ' ') {
      words[index++] = temp;
      temp = "";
    } else {
      temp += msg[i];
    }
  }
  words[index] = temp;

  // Auto text size
  int textSize = (wordCount > 3) ? 1 : 2;
  display.setTextSize(textSize);

  int lineHeight = 8 * textSize;
  int totalHeight = lineHeight * wordCount;
  int startY = (64 - totalHeight) / 2;

  for (int i = 0; i < wordCount; i++) {
    int16_t x1, y1;
    uint16_t w, h;

    display.getTextBounds(words[i], 0, 0, &x1, &y1, &w, &h);

    int xPos = (128 - w) / 2;
    int yPos = startY + (i * lineHeight);

    display.setCursor(xPos, yPos);
    display.println(words[i]);
  }

  display.display();
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

void setup() {
  Serial.begin(9600);

  pinMode(R, OUTPUT);
  pinMode(Y, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(sensor, INPUT);

  // ---- OLED INIT ----
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Not Found!");
    while (true);
  }

  showMessage("System Initializing...");
  delay(1500);

  // ---- SERVO INIT ----  
  servo.attach(SERVO_PIN);  // ESP8266 default pulse OK

  servo.write(110);  // Gate closed
  showMessage("Waiting for Motion");
}

void loop() {
  bool value = digitalRead(sensor);
  Serial.println(value);

  if (value == HIGH) {

    delay(500);  // Debounce PIR

    if (digitalRead(sensor) == HIGH) {

      digitalWrite(R, HIGH);
      showMessage("Motion Detected!");

      // ---- Opening Gate ----
      showMessage("Gate Opening...");
      smoothServoMove(110, 180, 30);

      digitalWrite(G, HIGH);
      digitalWrite(R, LOW);

      showMessage("Hold 3_Secs");
      delay(3000);

      // ---- Warning Before Closing ----
      showMessage("Warning! Closing Soon");
      digitalWrite(G, LOW);
      blinkWarningLED();

      // ---- Closing Gate ----
      showMessage("Gate Closing...");
      digitalWrite(R, HIGH);
      smoothServoMove(180, 110, 30);
      digitalWrite(R, LOW);

      showMessage("Waiting for Motion");
    }
  }

  delay(200);
}
