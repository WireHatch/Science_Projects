#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PulseSensorPlayground.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "spo2_algorithm.h"
#include <WiFi.h>
#include <time.h>
#include <ESP_Google_Sheet_Client.h>

// ===============================
// WiFi CREDENTIALS - MODIFY HERE
// ===============================
const char* ssid = "laha";
const char* password = "cuetmie22";

// ===============================
// NTP SERVER SETTINGS
// ===============================
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 6 * 3600;  // Bangladesh Time (GMT+6)
const int daylightOffset_sec = 0;

// ===============================
// HARDWARE PIN DEFINITIONS - MODIFY FOR YOUR SETUP
// ===============================
#define SELECT_B 11      // Select button pin
#define RESET_B 12       // Reset button pin  
#define SCROLL_B 10      // Scroll button pin
#define DS18B20_PIN 2    // DS18B20 temperature sensor pin
#define PULSE_PIN 0      // Pulse sensor analog pin
#define THRESHOLD 550    // Pulse sensor threshold - ADJUST FOR SENSITIVITY

// ===============================
// SENSOR INITIALIZATION
// ===============================
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD address and size
MAX30105 particleSensor;              // MAX30102 sensor for SpO2
OneWire oneWire(DS18B20_PIN);         // OneWire instance for DS18B20
DallasTemperature sensors(&oneWire);  // Dallas Temperature instance
PulseSensorPlayground pulseSensor;    // Pulse sensor instance

// ===============================
// MEASUREMENT VARIABLES
// ===============================
float temperatureC, temperatureF;    // Temperature readings
int heartRate = 0;                   // Heart rate reading
int spo2 = 0;                        // SpO2 reading
bool measurementsComplete = false;   // Flag for complete measurements

// ===============================
// MAX30102 SENSOR CONFIGURATION - MODIFY FOR SENSITIVITY
// ===============================
#define MAX_BRIGHTNESS 255
uint32_t irBuffer[100];              // IR LED sensor data
uint32_t redBuffer[100];             // Red LED sensor data
int32_t bufferLength = 100;          // Buffer size for calculations
int8_t validSPO2;                    // Valid SpO2 flag
int8_t validHeartRate;               // Valid heart rate flag

// ===============================
// BUTTON STATE VARIABLES
// ===============================
bool lastScrollState = HIGH;
bool lastSelectState = HIGH;
bool lastResetState = HIGH;
unsigned long lastResetPressTime = 0;
const unsigned long resetConfirmWindow = 3000; // 3 seconds for reset confirmation

// ===============================
// MENU SYSTEM VARIABLES
// ===============================
int currentMenuIndex = 0;
const char* menuItems[] = {"Start Measurement", "View Report", "Settings"};
const int menuItemCount = sizeof(menuItems) / sizeof(menuItems[0]);

// ===============================
// STATE MACHINE DEFINITIONS
// ===============================
enum SystemState {
  IDLE,
  SHOW_MENU,
  PLACE_FINGER_ON_SENSOR,
  INITIALIZING_MEASUREMENT,
  MEASURING_TEMPERATURE,
  MEASURING_PULSE,
  MEASURING_SPO2,
  SHOWING_REPORT,
  UPLOADING_DATA,
  AWAITING_CANCEL_CONFIRMATION
};

SystemState currentState = IDLE;
SystemState previousStateBeforeCancel = IDLE;
unsigned long stateStartTime = 0;

// ===============================
// GOOGLE SHEETS CONFIGURATION - MODIFY WITH YOUR CREDENTIALS
// ===============================
#define PROJECT_ID "storied-courier-459104-d3"
#define CLIENT_EMAIL "data-logging@storied-courier-459104-d3.iam.gserviceaccount.com"
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQDfhnwpBds9xdl/\nUgKp3Yz4reEGrsSylN8uX+Oae0QcSuRiyS3dD2j2iQrySKfriA+GLsytvSSwCXG/\naiKDUNV8v1fXR+g4U4YFCrO1AVxNiSPeWHR/4nPyYdFk/ClLOtrp1pkKaAgBESRm\nKX8kGqgIT7V9ONUDdIQ0iWH8jVMb4nFs4khT1OMJO7s8cA5LpB37DRWqixgJ+bX7\nSQDt+eMkhjz7EGJ5yY6a12XKuJSMMe0MFpZt58vyn2JG1TBs97joHhSalYmnsOkq\nolwOZLnLQwfMp+JMJ9dk+dGjE0o/IPwpH6vgSBNrsShvqYToHkBkVuMLvdh+RHh1\nL/JCafMpAgMBAAECggEAHoezObSFEiuN3igIw8QbrHOuF/LoXC6dSoDqXiqWo6P5\ntyHHU+RLmoYeqqJl3R5uZbjKrV2iPzEVtOaoOXYMUsedFdh1IrcLHRvYbnv1wYQJ\nFCYnc2LYzLMI81lwbR1oJ9GZ1vldQ9gK4rFUSPgjGfoueHYPZ4i97fcTHSL9xlb8\nJut2jUMQrylgdg4Vwes1KEB9wUaB6YNB0ehF11yaQmHueQdMeUSqIK3bRCXLOgCa\nuXR2da47GhDjtIE1yoVBX1jb1LlBuTxq9PuUPd91sLNcjfuvBEzd0VVvQVVuYYPL\n0ve/YPSLq2HrN0sbzjaBVE0Wv1hlT+OVRzzS2Hk2CQKBgQDxlmUIR0eiTOS7Qc+j\nQxJ2PRvaAj3Jd/eJ++nXvJKvmkene/RkKCLCuaauT4j92nGZLTC5FZ5tNqBst1av\n3lAApsjpaI6V22QS+CLSvi0U4aDuUrFs0XGUur+Omn/nP31J7unFySd47mkhkR8I\nN2d+E8lKAN5mRnnEmL3xJAhUEwKBgQDs3D0474tPRtCdMecQLv9jRaoisAQnlGV8\nq77Nr7P9vtyAtMSNONBlkiSSvaOQ+vhEBm0Qg5qgVQoZsGBBRAArlm/dm8ZJWjlO\nuyEa9KHuT/gDb4u8k7/AfsdB8Z6f8d5hoeKNFsRrlsPwabbD4h/Om7f4g0ytB1HA\nkuKzk/qrUwKBgAG+XSJPKWQxe+EiG3aJMGPsEvxi0cFXfVDgoZJuCPz9dxDtYxkp\n9P5EysAtUMbMQc+iNNzRHCCbqVK/i/+e8iLfEUol+Al62OEOK2or9zVUX434yNEJ\nPfGd224TDWNE4QmckNFdn0xPJDEa1AeCJtwRFn3TPcetMInLtGs+IFVDAoGAdPFk\nFMlISbLX6tSplXbuxq+tiYTH+FavKLeO8/Ss9DoYmj8lz+feFFfsULrql/khPEzD\n6mRfoot5+ib3zyFoyBx1TL/NTYgLRVDkzm87x/KZq/CV2FxGTx8I+Aj4XVXIiwGn\nXiuAfPIp+XfoMZxiQdrmIP+Kit9TQLYE0c7c81sCgYBLMI1NUU6wnoNRuSKSO6ow\nbXzuGlQI4aoTJZNTR7IoyMnkDM0adKGsbFic0tlWolZgvh36QcF1z5JGIQpYxdCe\nc8TZVN1owjUdjM7j45Bza21w3PfekO3YYo9mOoKy4KVPhAtptmmeijNAK0IWHisB\nwbHOSTeq1oEMrftpvq2BHg==\n-----END PRIVATE KEY-----\n";
const char spreadsheetId[] = "1kDjq3gQimDi8B4tp56XdjAnZglNqGsr7WYUBu8dAYOA";
unsigned long lastUploadTime = 0;
const unsigned long uploadInterval = 60000; // Upload every 60 seconds

// ===============================
// FUNCTION DECLARATIONS
// ===============================
void updateStateMachine();
void waitForFinger();
void initializeMeasurement();
void measureTemperature();
void measurePulse();
void measureOxygen();
void showReport();
void uploadToGoogleSheets();
void displayMenu();
void handleMenuSelection();
void updateTimeOnMenuScreen();
void resetDeviceState();
void displayCancelConfirmation();
String getCurrentTime();
void countdown(int seconds);
void tokenStatusCallback(TokenInfo info);
void updateLCD(const String &line1, const String &line2);
bool isButtonPressed();
bool isResetButtonPressed();
bool isSensorTouched();
void spinner();

// ===============================
// SETUP FUNCTION
// ===============================
void setup() {
  Serial.begin(115200);
  
  // Initialize button pins as INPUT_PULLUP
  pinMode(SELECT_B, INPUT_PULLUP);
  pinMode(RESET_B, INPUT_PULLUP);
  pinMode(SCROLL_B, INPUT_PULLUP);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  updateLCD("Health Monitor v1.0", "Initializing...");
  
  // Initialize temperature sensor
  sensors.begin();
  
  // Initialize pulse sensor
  pulseSensor.analogInput(PULSE_PIN);
  pulseSensor.setThreshold(THRESHOLD);
  if (pulseSensor.begin()) {
    Serial.println("Pulse Sensor initialized successfully");
  }
  
  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  int wifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
    delay(500);
    spinner();
    Serial.print(".");
    wifiAttempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected successfully");
    updateLCD("WiFi Connected", "Starting sensors...");
    
    // Initialize NTP time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    
    // Initialize Google Sheets
    GSheet.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);
    GSheet.setTokenCallback(tokenStatusCallback);
    GSheet.setPrerefreshSeconds(10 * 60);
    GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);
  } else {
    Serial.println("\nWiFi connection failed");
    updateLCD("WiFi Failed", "Continuing offline");
  }
  
  // Initialize MAX30102 sensor
  Serial.println("Initializing MAX30102...");
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30102 not found!");
    updateLCD("MAX30102 Error", "Check wiring");
    while (1); // Stop execution if sensor not found
  }
  
  // Configure MAX30102 sensor - MODIFY THESE VALUES FOR DIFFERENT SENSITIVITY
  byte ledBrightness = 60;    // 0=Off to 255=50mA
  byte sampleAverage = 4;     // 1, 2, 4, 8, 16, 32
  byte ledMode = 2;           // 1=Red only, 2=Red+IR, 3=Red+IR+Green
  byte sampleRate = 100;      // 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411;       // 69, 118, 215, 411
  int adcRange = 4096;        // 2048, 4096, 8192, 16384
  
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
  
  delay(2000);
  updateLCD("System Ready", "Touch sensor/button");
  currentState = IDLE;
  stateStartTime = millis();
}

// ===============================
// MAIN LOOP
// ===============================
void loop() {
  // Handle reset button with confirmation (highest priority)
  if (isResetButtonPressed()) {
    handleResetButton();
    return; // Exit early if reset is being handled
  }
  
  // Handle cancel confirmation timeout
  if (currentState == AWAITING_CANCEL_CONFIRMATION && 
      (millis() - lastResetPressTime >= resetConfirmWindow)) {
    Serial.println("Cancel confirmation timed out");
    currentState = previousStateBeforeCancel;
    if (currentState == SHOW_MENU) {
      displayMenu();
    }
  }
  
  // Update state machine (skip if awaiting cancel confirmation)
  if (currentState != AWAITING_CANCEL_CONFIRMATION) {
    updateStateMachine();
  }
  
  // Background Google Sheets upload
  if (GSheet.ready() && measurementsComplete && 
      (millis() - lastUploadTime >= uploadInterval)) {
    uploadToGoogleSheets();
    lastUploadTime = millis();
    measurementsComplete = false;
  }
  
  delay(50); // Small delay to prevent excessive CPU usage
}

// ===============================
// STATE MACHINE UPDATE
// ===============================
void updateStateMachine() {
  switch (currentState) {
    case IDLE:
      // Wait for sensor touch or button press to start
      updateTimeOnMenuScreen();
      lcd.setCursor(0, 1);
      lcd.print("Ready...        ");
      
      // Check for finger detection OR button press - MAIN TRIGGER CONDITIONS
      if (isSensorTouched() || isButtonPressed()) {
        Serial.println("Starting measurement sequence");
        currentState = PLACE_FINGER_ON_SENSOR;
        measurementsComplete = false;
        stateStartTime = millis();
      }
      break;
      
    case SHOW_MENU:
      displayMenu();
      handleMenuSelection();
      break;
      
    case PLACE_FINGER_ON_SENSOR:
      waitForFinger();
      break;
      
    case INITIALIZING_MEASUREMENT:
      initializeMeasurement();
      break;
      
    case MEASURING_TEMPERATURE:
      measureTemperature();
      break;
      
    case MEASURING_PULSE:
      measurePulse();
      break;
      
    case MEASURING_SPO2:
      measureOxygen();
      break;
      
    case SHOWING_REPORT:
      showReport();
      break;
      
    case UPLOADING_DATA:
      uploadToGoogleSheets();
      break;
  }
}

// ===============================
// FINGER DETECTION AND WAITING
// ===============================
void waitForFinger() {
  long irValue = particleSensor.getIR();
  bool fingerDetected = (irValue > 50000); // MODIFY THRESHOLD FOR FINGER DETECTION
  
  // Request temperature reading
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  bool tempValid = (tempC > 0 && tempC < 50); // Valid temperature range
  
  // Check pulse sensor
  bool pulseDetected = pulseSensor.sawStartOfBeat();
  
  if (fingerDetected || tempValid || pulseDetected) {
    updateLCD("Finger Detected!", "Starting in 3s...");
    countdown(3);
    currentState = INITIALIZING_MEASUREMENT;
    stateStartTime = millis();
  } else {
    updateLCD("Place finger on", "sensor to begin");
  }
}

// ===============================
// MEASUREMENT INITIALIZATION
// ===============================
void initializeMeasurement() {
  updateLCD("Initializing", "measurements...");
  
  if (millis() - stateStartTime >= 2000) {
    currentState = MEASURING_TEMPERATURE;
    stateStartTime = millis();
  }
}

// ===============================
// TEMPERATURE MEASUREMENT
// ===============================
void measureTemperature() {
  // Check if finger is still present
  if (!isSensorTouched()) {
    updateLCD("Finger removed!", "Please replace");
    delay(1000);
    currentState = PLACE_FINGER_ON_SENSOR;
    return;
  }
  
  updateLCD("Measuring", "Temperature...");
  
  // Get temperature reading
  sensors.requestTemperatures();
  temperatureC = sensors.getTempCByIndex(0);
  temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  
  // Display temperature
  lcd.setCursor(0, 1);
  lcd.print("T: ");
  lcd.print(temperatureF, 1);
  lcd.print("F");
  
  if (millis() - stateStartTime >= 3000) {
    currentState = MEASURING_PULSE;
    stateStartTime = millis();
  }
}

// ===============================
// PULSE MEASUREMENT
// ===============================
void measurePulse() {
  updateLCD("Measuring", "Heart Rate...");
  
  // Get heart rate from pulse sensor
  int bpm = pulseSensor.getBeatsPerMinute();
  
  if (pulseSensor.sawStartOfBeat()) {
    heartRate = bpm;
    lcd.setCursor(0, 1);
    lcd.print("HR: ");
    lcd.print(heartRate);
    lcd.print(" BPM");
    
    if (millis() - stateStartTime >= 5000) {
      currentState = MEASURING_SPO2;
      stateStartTime = millis();
    }
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Detecting...");
  }
}

// ===============================
// OXYGEN SATURATION MEASUREMENT
// ===============================
void measureOxygen() {
  updateLCD("Measuring", "Oxygen Level...");
  
  // Fill buffers with sensor data
  for (byte i = 0; i < bufferLength; i++) {
    while (!particleSensor.available()) {
      particleSensor.check();
    }
    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample();
  }
  
  // Calculate SpO2
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, 
                                         &spo2, &validSPO2, &heartRate, &validHeartRate);
  
  if (validSPO2 && spo2 > 0) {
    lcd.setCursor(0, 1);
    lcd.print("SpO2: ");
    lcd.print(spo2);
    lcd.print("%");
    
    if (millis() - stateStartTime >= 5000) {
      measurementsComplete = true;
      currentState = SHOWING_REPORT;
      stateStartTime = millis();
    }
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Calculating...");
  }
}

// ===============================
// SHOW MEASUREMENT REPORT
// ===============================
void showReport() {
  updateLCD("Health Report", "");
  
  // Display all measurements on one line (for 16x2 LCD)
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temperatureF, 0);
  lcd.print("F HR:");
  lcd.print(heartRate);
  lcd.print(" O2:");
  lcd.print(spo2);
  lcd.print("%");
  
  if (millis() - stateStartTime >= 10000) {
    currentState = IDLE;
    stateStartTime = millis();
  }
}

// ===============================
// GOOGLE SHEETS UPLOAD
// ===============================
void uploadToGoogleSheets() {
  if (!GSheet.ready()) {
    Serial.println("Google Sheets not ready");
    return;
  }
  
  updateLCD("Uploading to", "Google Sheets...");
  
  String currentTime = getCurrentTime();
  FirebaseJson response;
  FirebaseJson valueRange;
  
  valueRange.add("majorDimension", "ROWS");
  valueRange.set("values/[0]/[0]", currentTime);
  valueRange.set("values/[0]/[1]", temperatureF);
  valueRange.set("values/[0]/[2]", heartRate);
  valueRange.set("values/[0]/[3]", spo2);
  
  bool success = GSheet.values.append(&response, spreadsheetId, "Sheet1!A1", &valueRange);
  
  if (success) {
    Serial.println("Data uploaded successfully");
    updateLCD("Upload", "Successful!");
  } else {
    Serial.println("Upload failed");
    updateLCD("Upload", "Failed!");
  }
  
  delay(2000);
}

// ===============================
// UTILITY FUNCTIONS
// ===============================

// Check if any sensor is touched - MODIFY SENSITIVITY HERE
bool isSensorTouched() {
  long irValue = particleSensor.getIR();
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  bool pulseDetected = pulseSensor.sawStartOfBeat();
  
  return (irValue > 50000) || (tempC > 0 && tempC < 50) || pulseDetected;
}

// Check if any button is pressed
bool isButtonPressed() {
  return (digitalRead(SELECT_B) == LOW) || 
         (digitalRead(SCROLL_B) == LOW) || 
         (digitalRead(RESET_B) == LOW);
}

// Check if reset button is pressed
bool isResetButtonPressed() {
  return digitalRead(RESET_B) == LOW;
}

// Handle reset button with confirmation
void handleResetButton() {
  unsigned long currentMillis = millis();
  
  if (currentState == AWAITING_CANCEL_CONFIRMATION) {
    if (currentMillis - lastResetPressTime < resetConfirmWindow) {
      Serial.println("Reset confirmed!");
      resetDeviceState();
      currentState = IDLE;
      return;
    }
  }
  
  Serial.println("Reset button pressed - awaiting confirmation");
  previousStateBeforeCancel = currentState;
  currentState = AWAITING_CANCEL_CONFIRMATION;
  displayCancelConfirmation();
  lastResetPressTime = currentMillis;
}

// Reset all device state
void resetDeviceState() {
  measurementsComplete = false;
  temperatureC = 0;
  temperatureF = 0;
  heartRate = 0;
  spo2 = 0;
  currentMenuIndex = 0;
  Serial.println("Device state reset");
}

// Display cancel confirmation
void displayCancelConfirmation() {
  updateLCD("Cancel operation?", "Press RESET again");
}

// Display menu
void displayMenu() {
  updateTimeOnMenuScreen();
  lcd.setCursor(0, 1);
  lcd.print(menuItems[currentMenuIndex]);
  lcd.print(" <");
}

// Handle menu selection
void handleMenuSelection() {
  bool scrollPressed = (digitalRead(SCROLL_B) == LOW) && (lastScrollState == HIGH);
  bool selectPressed = (digitalRead(SELECT_B) == LOW) && (lastSelectState == HIGH);
  
  if (scrollPressed) {
    currentMenuIndex = (currentMenuIndex + 1) % menuItemCount;
    delay(200); // Debounce
  }
  
  if (selectPressed) {
    switch (currentMenuIndex) {
      case 0: // Start Measurement
        currentState = PLACE_FINGER_ON_SENSOR;
        measurementsComplete = false;
        break;
      case 1: // View Report
        currentState = SHOWING_REPORT;
        break;
      case 2: // Settings
        // Add settings functionality here
        break;
    }
    delay(200); // Debounce
  }
  
  lastScrollState = digitalRead(SCROLL_B);
  lastSelectState = digitalRead(SELECT_B);
}

// Update time on menu screen
void updateTimeOnMenuScreen() {
  String currentTime = getCurrentTime();
  lcd.setCursor(0, 0);
  if (currentTime != "Time unavailable") {
    lcd.print(currentTime.substring(11, 19)); // Show only time HH:MM:SS
  } else {
    lcd.print("System Ready");
  }
}

// Get current time string
String getCurrentTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "Time unavailable";
  }
  char timeStr[30];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(timeStr);
}

// Countdown function
void countdown(int seconds) {
  for (int i = seconds; i > 0; i--) {
    lcd.setCursor(0, 1);
    lcd.print("Starting in ");
    lcd.print(i);
    lcd.print("s   ");
    delay(1000);
  }
}

// Update LCD with two lines
void updateLCD(const String &line1, const String &line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

// Spinner animation
void spinner() {
  static int counter = 0;
  const char* glyphs = "|/-\\";
  lcd.setCursor(15, 1);
  lcd.print(glyphs[counter++ % 4]);
}

// Token status callback for Google Sheets
void tokenStatusCallback(TokenInfo info) {
  Serial.printf("Token info: type = %s, status = %s\n", 
                GSheet.getTokenType(info).c_str(), 
                GSheet.getTokenStatus(info).c_str());
  if (info.status == token_status_error) {
    Serial.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
  }
}
