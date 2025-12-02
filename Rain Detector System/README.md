# Smart Rain-Activated Servo System (ESP8266)

## Overview
This project is a rain-activated automatic servo system using:

- NodeMCU (ESP8266)
- Digital Rain Sensor Module
- Servo Motor (SG90 / MG996R)

When rain is detected, the system:

1. Reads the rain sensor digital signal
2. Opens the servo to a defined angle
3. Closes the servo when rain stops

This system can be used for:
- Auto-closing window covers
- Rain-protection lids
- Smart agriculture canopy systems
- Weather-responsive automation

---

## 🛠 Used Components
| Component | Quantity |
|----------|----------|
| NodeMCU ESP8266 | 1 pcs |
| Rain Sensor (Digital Output) | 1 pcs |
| SG90 / MG996R Servo Motor | 1 pcs |
| Jumper Wires | – |
| Optional: External 5V supply for servo | – |

---

## 🔌 Pin Connections (NodeMCU)
| Component | NodeMCU Pin | GPIO |
|-----------|-------------|------|
| Rain Sensor (DO) | D3 | GPIO0 |
| Servo Signal | D4 | GPIO2 |

### ⚠ Important:
- **Never power the servo from NodeMCU 3.3V pin.**
- Use a **5V external supply** for the servo.
- Connect **all grounds together** (NodeMCU GND + Servo GND + Rain Sensor GND).

---

## 🎬 Stages of Operation

### 1. Idle (No Rain)
- Rain sensor OUT = HIGH  
- Servo moves to **closed position**  
- System continuously monitors rainfall

---

### 2. Rain Detected
- Rain sensor OUT = LOW  
- Servo moves to **open position**  
- System stays open as long as rain = detected

---

### 3. Rain Stopped
- Rain sensor OUT = HIGH again  
- Servo returns to **closed position**

---

## 📦 Optional Future Upgrades
- Add **SSD1306 OLED display** for rain status
- Add **buzzer alarm**
- Add **Wi-Fi notification**
- Add **Rain intensity (analog read)** mode
- Add **manual override switch**

---

## 📘 Summary
This system is a simple but effective **rain-triggered automation mechanism** using ESP8266 + Servo.  
Perfect for beginners building weather-responsive IoT prototypes.

