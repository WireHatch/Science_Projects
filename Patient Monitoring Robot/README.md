# ESP32 Health Monitoring System  
### (MAX30102 | DS18B20 | LCD | Multi-Stage State Machine)

---

## 📌 Overview

This project is a smart **health monitoring and medication dispensing system** built using an **ESP32 Microcontroller**.  

It performs the following automated health measurements:

- **Body Temperature** using DS18B20
- **Heart Rate (BPM)** using MAX30102 IR-based optical sensor
- **Blood Oxygen Saturation (SpO2%)** using the MAX30102 sensor algorithm

After measurement, the system displays a summary report on an **I2C LCD display** and supports a servo-driven **automatic pill dispensing mechanism**.

User interaction happens through a **three-button control interface** supporting:

| Button Function | Description |
|----------------|-------------|
| NEXT / START | Begin or advance process |
| RESTART / CONFIRM | Reset measurement or confirm restart |
| HOLD | Pause ongoing process |

A non-blocking **state machine** runs the workflow including finger detection, measurement sequencing, reporting, and restart handling.

---

## 🔌 Pin Connections

| Component | Connection | ESP32 Pin |
|----------|------------|-----------|
| **Servo Motor** | Signal | GPIO 13 |
| **DS18B20 Temperature Sensor** | Data | GPIO 4 |
| **MAX30102 Sensor** | SDA / SCL | GPIO 21 / GPIO 22 |
| **LCD Display (I2C)** | SDA / SCL | GPIO 21 / GPIO 22 |
| **Button A (Next / Start)** | Input Pull-Up | GPIO 15 |
| **Button B (Restart / Confirm)** | Input Pull-Up | GPIO 14 |
| **Button C (Hold)** | Input Pull-Up | GPIO 12 |

---

## 🧠 System Stages (State Machine Flow)

| Stage | Description |
|--------|------------|
| **IDLE** | System waiting for user input |
| **SHOW MENU** | Prompt user to begin measurement |
| **PLACE_FINGER_ON_SENSOR** | Checks for valid finger placement on MAX30102 |
| **INITIALIZING_MEASUREMENT** | Prepares sensors & captures stable readings |
| **MEASURING_TEMPERATURE** | Reads body temperature |
| **MEASURING_PULSE** | Detects real heartbeat and calculates BPM |
| **MEASURING_SPO2** | Estimates oxygen saturation level |
| **SHOWING_REPORT** | Summary shown on LCD |
| **AWAITING_CANCEL_CONFIRMATION** | Confirms restart request |

(Additional paused states handled using button "HOLD".)

---

## 📡 Sensors and Working Principles

### 1️⃣ MAX30102 (Heart Rate & SpO₂)

- Uses **optical PPG (Photoplethysmography)**.
- Emits red/IR light and measures intensity changes from blood flow.
- Detects:
  - **Heart Rate (BPM)** based on time between peaks.
  - **SpO₂** ratio using absorption differences of red vs infrared wavelengths.

📍 **Finger detection is performed using IR signal threshold** (>50,000).

---

### 2️⃣ DS18B20 Temperature Sensor

- Uses **1-Wire digital communication**.
- Measures temperature with **±0.5°C accuracy**.
- Reads temperature every cycle using the DallasTemperature library.

---

## 🔄 Process Summary

1. **User presses NEXT** → System initializes.
2. **System detects finger** using IR sensor.
3. **Temperature is measured**.
4. **Heart rate is measured** using peak-to-peak timing.
5. **SpO₂ is estimated**.
6. **Results displayed on LCD**.
7. (Optional) **Hold / Restart ** available anytime.

---

## 🛠 Libraries Used

| Library | Function |
|--------|----------|
| `Wire.h` | I2C communication |
| `LiquidCrystal_I2C.h` | LCD interfacing |
| `DallasTemperature.h` & `OneWire.h` | DS18B20 sensing |
| `MAX30105.h`, `spo2_algorithm.h`, `heartRate.h` | MAX30102 processing |

---

## 🚀 Features

- Interactive 3-Button Control
- Auto finger detection
- State-driven workflow
- Debouncing for all buttons
- Safeguards for invalid sensor readings
- Resume / Hold capability

---

## 📄 Summary

This program combines medical sensing with automation using a structured state machine. It can serve as a part of:

- Remote healthcare systems  
- Smart medication assistants  
- IoT-based health monitoring platforms  

---

### ✔ Ready to Upload & Run on ESP32.

