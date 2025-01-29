![Photo01](https://github.com/kysutrung/plant_potz/blob/main/mediaaaa/repo_cover1.jpg)

# ALO ALO SỬA COVER ĐÊĐÊ

# IoT Smart Irrigation System using ESP8266

## Introduction
This project is an IoT-based smart irrigation system that monitors soil moisture, temperature and humidity to automate plant watering. The system utilizes an ESP8266 microcontroller, soil moisture sensors and temperature & humidity sensors. Users can control the system manually or set it to automatic mode for efficient water usage.

## Features
- **Automatic Mode**: The system autonomously waters plants based on soil moisture levels.
- **Manual Mode**: Users can control the pump remotely via a web-based interface.
- **Real-time Monitoring**: Displays live soil moisture, temperature, and humidity data.
- **WiFi Connectivity**: The ESP8266 connects to a web dashboard for remote monitoring and control.
- **Energy Efficient**: Optimized water usage to prevent overwatering.

## Hardware Requirements
- ESP8266 (NodeMCU or Wemos D1 Mini)
- LM393 Soil Moisture Sensor
- DHT21 Temperature & Humidity Sensor
- Relay Module
- Water Pump
- 12V Power Supply (for pump)

## Software Requirements
- Arduino IDE with ESP8266 Board Manager installed
- Blynk or MQTT (for remote control & monitoring)
- Libraries:
  - `ESP8266WiFi.h`
  - `BlynkSimpleEsp8266.h` (if using Blynk)
  - `DHT.h` (for temperature & humidity sensor)

## Usage
1. Power on the system.
2. Access the web dashboard to view sensor data.
3. Toggle between **Manual** and **Automatic** mode.
4. In **Automatic Mode**, the pump activates when the soil moisture is low.
5. In **Manual Mode**, control the pump via the dashboard.

## Troubleshooting
- Ensure WiFi credentials are correctly configured.
- Check power supply connections for the pump.
- Verify sensor readings using the serial monitor.
- Ensure correct GPIO pin mappings in the code.

