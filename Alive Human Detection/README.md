# PIR-Based Smart Home System (ESP32 + ESP8266)

A universal motion-activated LED system compatible with both **ESP32 and ESP8266**.  
When the PIR detects movement, lights turn on, then all LEDs stay ON at low brightness.  
If no movement is detected for 5 seconds, the LEDs perform a **warning blink** and then turn OFF.


---

## 🛠 Features
- Universal: Works on **ESP32 & ESP8266**
- PIR motion detection (HC-SR501)
- Accelerating blinking warning before shutdown
- Auto-reset on next detection
- Debounce and timer-controlled fade logic
---

## 📦 Components Required
| Component | Quantity | Description |
|----------|----------|-------------|
| ESP8266 (NodeMCU) **or** ESP32 | 1 | Main Microcontroller |
| HC-SR501 PIR Motion Sensor | 1 | Detects human presence |
| LEDs (Any color) |  | Output indicators |
| Jumper Wires | — | Connections |

---

## 🔌 Pin Connections

### ⚙ ESP8266 Pins
| Purpose | ESP8266 Pin |
|---------|-------------|
| LED1 | D5 (GPIO 14) |
| LED2 | D6 (GPIO 12) |
| LED3 | D7 (GPIO 13) |
| PIR Sensor OUT | D1 (GPIO 5) |

### ⚙ ESP32 Pins
| Purpose | ESP32 Pin |
|---------|-----------|
| LED1 | GPIO 26 |
| LED2 | GPIO 27 |
| LED3 | GPIO 25 |
| PIR Sensor OUT | GPIO 12 |

---

## 🔄 System Stages (Operation Flow)

### **1️⃣ Boot Stage**
- All LEDs are OFF  
- System prints: “System Ready – Waiting for Motion…”

### **2️⃣ Motion Detected**
- PIR output HIGH  
- Chaser animation runs:  
- LED1 → LED1+LED2 → LED1+LED2+LED3  
- Then all LEDs stay ON at low brightness

### **3️⃣ Continuous Motion**
- Every new motion resets timer  
- LEDs remain ON forever until no motion

### **4️⃣ No Motion for 5 Seconds**
- System begins warning stage:
- LEDs blink together
- Blink rate increases each cycle (accelerating)

### **5️⃣ Shutdown**
- All LEDs OFF  
- System resets and waits for the next motion

---

## ⚙ HC-SR501 PIR Sensor Adjustments

The PIR module has **two potentiometers** and **one mode jumper**.  
Proper adjustment is necessary for this code to work correctly.

---

### 🔧 1. Sensitivity Adjustment (Sx)

Controls **distance range** of detection.

- Turn **clockwise** → Increase range (up to ~7 meters)  
- Turn **counter-clockwise** → Decrease range (~3 meters minimum)

**Recommended:**  
Start from **middle position** and tune as needed.

---

### ⏱ 2. Time Delay Adjustment (Tx)

Controls how long PIR keeps output HIGH after detection.

- Clockwise → Long hold (up to 300 seconds)
- Counter-clockwise → Short hold (3–5 seconds)

⚠ **Important for this code:**  
Set **fully counter-clockwise (minimum)**  
Why?  
Because this system uses its **own 5-second timer**, and a long PIR signal would break the logic.

---

### 🔀 3. Trigger Mode Jumper (L / H)

| Mode | Behavior | Recommended? |
|------|----------|--------------|
| **H (Repeatable / Retriggerable)** | Output stays HIGH as long as motion continues | ✔ Yes |
| **L (Single Trigger)** | Goes HIGH once even if motion continues | ✖ No |

✔ **Set jumper to H mode** for perfect operation.

---
