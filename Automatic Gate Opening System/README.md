# 🔐 Smart Gate Control With PIR + Servo (NodeMCU Version)
![gate](https://github.com/user-attachments/assets/04d66ecf-7ca0-47da-ac90-04739867c773)

## 📌 Overview
This project is a **motion-activated smart gate system** using:
- NodeMCU (ESP8266)
- PIR motion sensor(https://lastminuteengineers.com/pir-sensor-arduino-tutorial/)
- Servo motor
- RGB LEDs for status indication

When motion is detected, the system:
1. Turns ON the Red LED (warning)
2. Opens the gate smoothly using a servo
3. Turns ON the Green LED (gate open)
4. Waits 3 seconds
5. Flashes the Yellow LED (warning)
6. Closes the gate smoothly

---

## 🛠 Used Components
- NodeMCU ESP8266 (1 pcs)
- PIR Sensor HC-SR501 (1 pcs)
- SG90 or MG996R Servo Motor (1 pcs)
- Red, Yellow, Green LEDs (3 pcs)
- 220Ω resistors (3 pcs)
- Jumper wires
- 5V external supply for servo (recommended)

---

## 🔌 Pin Connections (NodeMCU)

| Component | NodeMCU Pin | GPIO |
|----------|-------------|-------|
| Red LED        | D5 | GPIO14 |
| Yellow LED     | D6 | GPIO12 |
| Green LED      | D7 | GPIO13 |
| PIR Sensor OUT | D1 | GPIO5  |
| Servo Signal   | D4 | GPIO2  |
|   | Optional |   |
| SDA  | D1 | GPIO5  |
| SCL   | D2 | GPIO4 |

⚠ **Important:**  
Do NOT power the servo from the NodeMCU 3.3V pin.  
Use **external 5V** + common ground.

---

## 🎬 Stages of Operation
1. **Idle Mode:**  
   - Waiting for PIR motion.

2. **Motion Detected:**  
   - PIR = HIGH  
   - Red LED ON  
   - Gate starts opening.

3. **Gate Opening:**  
   - Servo moves from 110° → 180°  
   - Green LED ON  
   - Red LED OFF.

4. **Gate Open (Hold):**  
   - Wait 3 seconds.

5. **Warning Stage:**  
   - Green LED OFF  
   - Yellow LED blinks 5 times.

6. **Gate Closing:**  
   - Servo moves 180° → 110°  
   - Red LED briefly ON  
   - System returns to idle.

---

## ⚙ Notes

### 🖥 Screen Size
This project has **no display**, but you can add:
- OLED SSD1306  
- LCD 20x4  
- TFT Display  
If needed later.

### 🔧 Servo Calibration
- Default **closed** pos
