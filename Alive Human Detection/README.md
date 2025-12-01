# PIR-Based Automatic Light & Fan Control System using NodeMCU ESP8266

## Overview
This project uses a **PIR motion sensor** to automatically turn ON a **light and fan** whenever movement is detected in a room. After the person leaves, the system waits for a few seconds and then turns OFF both devices automatically.  

The goal is to save electricity by ensuring light & fan only run when needed — fully automatic and touch-free.

---

## Components Used
| Component | Quantity | Description |
|-----------|----------|-------------|
| NodeMCU ESP8266 | 1 | Main controller |
| PIR Sensor (HC-SR501) | 1 | Detects human presence |
| Relay Module (Single or Dual) | 1–2 | Controls AC Light + Fan |
| Light Bulb / Lamp | 1 | Load 1 |
| Fan (Any type) | 1 | Load 2 |
| Jumper Wires | — | For connections |
| Power Supply | 5V | For sensor + relay |

---

## Pin Connections

### ESP8266 → PIR Sensor
| PIR Pin | Connect To |
|---------|------------|
| VCC | 5V |
| GND | GND |
| OUT | D1 (GPIO 5) |

### ESP8266 → Relay Module
| Relay | Connect To |
|-------|------------|
| IN1 | D5 (GPIO 14) → Light |
| IN2 | D6 (GPIO 12) → Fan |
| VCC | 5V |
| GND | GND |

---

## Stages of Operation

### 1️⃣ Idle / Waiting Stage
- System monitors PIR sensor.
- Light + Fan remain **OFF**.

### 2️⃣ Motion Detected
- PIR output becomes HIGH.
- ESP8266 immediately:
  - Turns ON **Light**
  - Turns ON **Fan**

### 3️⃣ Active Stage
- As long as motion continues:
  - Light + Fan stay ON.
  - Timer resets on new motion.

### 4️⃣ No Motion Stage
- Countdown begins (e.g., 15 seconds).
- If no new motion is detected:
  - Light turns **OFF**
  - Fan turns **OFF**

### 5️⃣ Back to Idle
- System returns to waiting state.

---

## Notes
- Adjust `OFF_DELAY` in code for longer or shorter automatic OFF time.
- Use proper relay modules rated for your AC devices.
- Ensure proper isolation between ESP8266 and AC mains for safety.
