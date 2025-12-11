# 🌱 Automatic Plant Watering System using ESP8266

An IoT-based self-watering system that measures soil moisture and automatically controls a water pump using an L298N motor driver. This system helps maintain plant hydration without manual intervention.

---

## 📘 Overview
This project automatically waters plants by reading moisture levels from a soil sensor and activating a water pump when the soil becomes dry.  
The ESP8266 reads the analog moisture value and decides whether to start or stop the pump based on set threshold values.

---

## 🔧 Components Required
- **ESP8266 NodeMCU**
- **Soil Moisture Sensor (Analog)**
- **L298N Motor Driver**
- **Mini Water Pump (5–12V depending on your pump)**
- **Power Supply for Pump**
- **Jumper Wires**

---

## 🪛 Pin Connections

### **ESP8266 → Soil Moisture Sensor**
| Component | ESP8266 Pin |
|----------|-------------|
| Soil Sensor OUT | A0 |

### **ESP8266 → L298N Motor Driver**
| L298N Pin | ESP8266 Pin | Function |
|-----------|--------------|----------|
| IN1 | GPIO5 (D1) | Motor direction control |
| IN2 | GPIO4 (D2) | Motor direction control |
| ENA | GPIO0 (D3) | PWM speed control |

### **Pump → L298N**
- Motor Output terminals → Pump terminals
- External power supply → L298N +12V / GND
- **Common GND** between ESP8266 & L298N

---

## ⚙️ Working Stages

### **1️⃣ Initialization**
- Serial monitor starts.
- Motor driver pins set as output.
- Pump is stopped initially.
- Moisture monitoring begins.

### **2️⃣ Moisture Reading**
- ESP8266 reads analog moisture value from A0.
- Sensor values:
  - `> 800` → **Dry Soil**
  - `< 400` → **Wet Soil**
  - Between → **Normal Moisture**

### **3️⃣ Decision Making**
- If **DRY** → Pump **runs for 5 seconds**
- If **WET / OK** → Pump **stays OFF**

### **4️⃣ Watering Cycle**
- Pump turns ON.
- Runs for `WATERING_DURATION` (default 5 seconds).
- Turns OFF.
- System waits 30 seconds to allow water absorption.

### **5️⃣ Loop Continues**
- Every 10 seconds, moisture level is checked again.

---

## 📝 Notes
- Adjust `DRY_SOIL` and `WET_SOIL` based on your sensor calibration.
- Ensure the pump uses an **external power source** (motor drivers cannot run pumps directly from ESP8266).
- Always connect **GND of all devices together**.
- Soil moisture values vary with soil type—test and update thresholds accordingly.
- To extend pump life, avoid running it for too long or without water.

---

## 📂 Project Code
The full working source code is provided in the main `.ino` file.
