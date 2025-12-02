

# 📌 **ESP8266 Health Monitoring System with Blynk**

**Sensors:** DS18B20 Temperature Sensor, MAX30100 Pulse-Oximeter
**Display:** 16x2 LCD (I2C)
**Features:** Stage-wise measurement, cloud upload, alerts, timestamp logging

---

## 📝 **1. Overview**

This project is a **portable health monitoring device** built using the ESP8266 NodeMCU board that measures:

* **Body Temperature (°C / °F)**
* **Heart Rate (BPM)**
* **Oxygen Level SpO₂ (%)**

The device shows all vitals on the LCD **step-by-step**, saves data to **Blynk Cloud**, and triggers **alerts** if any reading becomes abnormal.

---

## ⚙️ **2. Hardware Used**

| Component                      | Purpose                            |
| ------------------------------ | ---------------------------------- |
| **ESP8266 NodeMCU**            | Main microcontroller + WiFi        |
| **DS18B20 Temperature Sensor** | Measures body temperature          |
| **MAX30100 Pulse Oximeter**    | Measures heart rate + blood oxygen |
| **16x2 LCD (I2C)**             | Displays values                    |
| **Push Button**                | Start measurement                  |
| **Wires + Breadboard**         | Basic connections                  |

---

## 🔌 **3. Pin Connections**

### **MAX30100 Pulse Oximeter**

| MAX30100 | NodeMCU    |
| -------- | ---------- |
| SDA      | D2 (GPIO4) |
| SCL      | D1 (GPIO5) |
| VIN      | 3.3V       |
| GND      | GND        |

### **DS18B20 Temperature Sensor**

| DS18B20 | NodeMCU    |
| ------- | ---------- |
| Signal  | D4 (GPIO2) |
| VCC     | 3.3V       |
| GND     | GND        |

### **LCD 16x2 (I2C)**

| LCD | NodeMCU    |
| --- | ---------- |
| SDA | D2 (GPIO4) |
| SCL | D1 (GPIO5) |
| VCC | 5V         |
| GND | GND        |

### **Push Button**

| Button     | NodeMCU    |
| ---------- | ---------- |
| One side   | D3 (GPIO0) |
| Other side | GND        |

---

## 📚 **4. Software / Libraries Required**

Add these from Arduino Library Manager:

* **Blynk**
* **DallasTemperature**
* **OneWire**
* **LiquidCrystal_I2C**
* **MAX30100_PulseOximeter**
* **ESP8266WiFi**

---

## 🚦 **5. System Workflow (Stages)**

### **Stage 1 — IDLE MODE**

* Waits for user button press or finger placement.
* Shows time and status on LCD.

### **Stage 2 — PLACE FINGER**

* Asks user to place finger on MAX30100.
* Auto-detects finger.

### **Stage 3 — TEMPERATURE MEASUREMENT (3 sec)**

* Reads body temperature from DS18B20.
* Displays °C and °F.

### **Stage 4 — HEART-RATE MEASUREMENT (10 sec)**

* Reads beats per minute using MAX30100.
* Shows stable value.

### **Stage 5 — OXYGEN LEVEL SpO₂ (10 sec)**

* Measures oxygen saturation.
* Shows percentage in real time.

### **Stage 6 — REPORT DISPLAY (10 sec)**

A summary is shown:

```
T:98°F HR:72 O:98
```

### **Stage 7 — CLOUD UPLOAD**

* Sends all readings to Blynk App:

  * Heart Rate
  * SpO₂
  * Temperature
  * Timestamp

* If unsafe values detected → **Blynk Alert Notification**

---

## 🚨 **6. Automatic Alerts**

The system checks:

| Vital       | Alert Condition     |
| ----------- | ------------------- |
| Heart Rate  | >120 BPM or <50 BPM |
| SpO₂        | <92%                |
| Temperature | >100.4°F or <95°F   |

Problems are reported instantly in Blynk.

---

# 🧠 **7. Simple Explanation for Non-Engineers**

(How these sensors work internally)

Here is an easy-to-understand version:

---

## 🧊 **DS18B20 Temperature Sensor — “Digital Thermometer Chip”**

* Inside the sensor, there is a **tiny electronic thermometer**.
* When it touches your skin, the chip senses heat.
* It turns that heat into numbers (°C/°F).
* The ESP8266 reads this number digitally—no analog signal, no noise.

➡️ **Think of it like a digital thermometer inside a USB pen-drive.**

---

## ❤️ **MAX30100 Pulse Oximeter — “Red & Infrared Light Trick”**

This is the same technology used in smartwatches and hospital pulse clips.

### Inside the sensor:

* A **red LED**
* An **infrared LED**
* A **light detector (photodiode)**

### How it works:

1. Red & IR light is shined into your finger.
2. Blood absorbs light differently depending on:

   * How much oxygen is in it (SpO₂)
   * How fast it pulses (heart rate)
3. The detector measures how much light comes back.
4. The sensor calculates:

   * Every beat → **Heart Rate**
   * Light absorption → **Oxygen Saturation (SpO₂)**

➡️ **It basically looks at your blood using red light—without cutting the skin.**

---

## 🧠 **LCD Display**

* The ESP8266 sends text to the LCD using only 2 wires (I2C).
* LCD shows all results live.

---

## ☁️ **Blynk Cloud**

* ESP8266 uses WiFi
* Sends your temperature, heart rate, and oxygen level
* Shows everything on your mobile dashboard instantly

---
