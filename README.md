# ESP8266 Environmental Data Logger

**Low-power, modular environmental data logging system with optional Wi-Fi web interface**

---

## Overview

This project is a **dual-mode ESP8266-based environmental data logger** designed for long-term, unattended monitoring of environmental parameters such as **temperature**, **barometric pressure**, and **soil humidity**.

The system can operate either in:

- **Low-power battery mode**, using deep sleep for extended autonomous operation  
- **Continuous powered mode**, providing Wi-Fi access and a built-in HTML web interface

This project demonstrates **embedded firmware development**, **hardware integration**, and **production-aware documentation**, and is suitable for prototyping, field testing, and small-scale deployments.

---

## Key Features

- ESP8266 controller (LoLin NodeMCU)
- Dual operating modes:
  - 🔋 **Battery-powered low-power mode** (deep sleep)
  - 🔌 **5V-powered Wi-Fi mode** with web interface
- Environmental sensing:
  - Temperature
  - Barometric pressure (BMP280)
  - Soil humidity (analog sensor)
- Data logging:
  - Local storage on MicroSD card (SPI)
- Timekeeping:
  - RTC module (I²C) for timestamped logs
- Modular design:
  - Sensors and peripherals easily replaceable
- Web-accessible data when externally powered

---

## Operating Modes

### 🔋 Battery Mode (Low Power)

- ESP8266 wakes periodically
- Reads all sensors
- Logs data to SD card
- Returns to deep sleep
- Approximate battery life: **~10 days** (configuration-dependent)

This mode is intended for **unattended data collection** where power consumption is critical.

---

### 🔌 Powered Mode (Wi-Fi Enabled)

- Continuous operation from external 5V supply
- ESP8266 runs as a Wi-Fi access point
- Embedded HTML page served directly by the ESP8266
- Enables data visualization and access without removing the SD card

---

## Hardware Overview
[![Schematic Preview](docs/schematic_preview.png)](docs/schematic.pdf)

### Main Components

- **LoLin NodeMCU (ESP8266)**
- **BMP280** temperature & pressure sensor (I²C)
- **DS1307 RTC module** (I²C, 3.3 V logic compatible)
- **MicroSD card adapter** (SPI)
- **Soil moisture sensor** (analog)

### Communication Interfaces

- **I²C** → BMP280, RTC  
- **SPI** → MicroSD card  
- **ADC** → Soil moisture sensor  

### Power

- 3.3 V regulated rail
- Battery or external 5 V supply
- Deep sleep wake-up via GPIO16 → RST

📄 Full wiring details are available in  
`hardware/wiring_schematic.pdf`

---

## Example Data Output

Logged data is stored on the SD card in a simple, readable format:

2026-02-09 12:00, 24.8°C, 1012 hPa, 42%
2026-02-09 12:15, 24.7°C, 1012 hPa, 43%


This format allows easy import into spreadsheets or data analysis tools.

---

## Firmware

The firmware is written in **Arduino C++** and built using **PlatformIO**.

### Firmware Features

- Deep sleep power management
- Modular sensor drivers
- SD card file logging
- Optional Wi-Fi web server
- Non-blocking design principles

### Build & Upload

cd firmware
pio run
pio run -t upload
pio device monitor
Platform Details

Framework: Arduino

Board: LoLin NodeMCU (ESP8266)

Toolchain: PlatformIO

Firmware-specific details are documented in
firmware/README.md.

ESP8266-Data-Logger/
├── README.md
├── firmware/
│   ├── platformio.ini
│   ├── src/
│   ├── include/
│   └── README.md
├── hardware/
│   └── wiring_schematic.pdf
└── images/

---
### Applications

This design can be adapted for:

- Environmental monitoring
- Agricultural soil sensing
- Remote sensor nodes

- Battery-powered data loggers

- IoT proof-of-concept systems

What I Can Build for You:

Based on this project, I can help with:

- Custom ESP8266 / ESP32 firmware

- Low-power IoT system design

- Sensor integration and data logging

- KiCad schematic and PCB design

- Prototype-to-production transition (JLCPCB-ready files)
---
### Author
Nick Christodoulou
 - Electronics & Firmware Engineer
 - Email: nichrist71@gmail.com

---
License

This project is released under the MIT License.
Contact me for commercial use, customization, or consulting work.


