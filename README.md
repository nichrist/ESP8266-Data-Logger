ESP8266 Environmental Data Logger

Low-power, modular environmental data logging system with optional Wi-Fi web interface

Overview

This project is a dual-mode ESP8266-based environmental data logger designed for long-term, unattended monitoring of environmental parameters such as temperature, barometric pressure, and soil humidity.

The system can operate either in low-power battery mode using deep sleep, or in continuous powered mode with Wi-Fi access and a built-in HTML web interface.

This project demonstrates embedded firmware design, hardware integration, and production-aware documentation, suitable for prototyping and small-scale deployments.

Key Features

ESP8266 (LoLin NodeMCU) controller

Dual operating modes:

Battery-powered low-power mode (deep sleep)

5V-powered Wi-Fi mode with web interface

Environmental sensing:

Temperature

Barometric pressure (BMP280)

Soil humidity (analog sensor)

Data logging:

Local storage on MicroSD card (SPI)

Timekeeping:

RTC module via I²C for timestamped logs

Modular design:

Sensor and peripheral modules easily replaceable

Web-accessible data when externally powered

Operating Modes

🔋 Battery Mode (Low Power)

ESP8266 wakes periodically

Reads sensors

Logs data to SD card

Returns to deep sleep

Approximate battery life: ~10 days (depending on configuration)


🔌 Powered Mode (Wi-Fi Enabled)

Continuous operation from 5V supply

ESP8266 runs as a Wi-Fi access point

Embedded HTML page for data visualization and access

Hardware Overview
Main Components

LoLin NodeMCU (ESP8266)

BMP280 temperature & pressure sensor (I²C)

DS1307 RTC module (I²C, 3.3 V logic compatible)

MicroSD card adapter (SPI)

Soil moisture sensor (analog)

Communication Interfaces

I²C: BMP280, RTC

SPI: MicroSD card

ADC: Soil moisture sensor

Power

3.3 V regulated rail

Battery or external 5 V supply

Deep sleep wake-up via GPIO16 → RST

📄 See hardware/wiring_schematic.pdf
 for full wiring details.

Firmware

The firmware is written in Arduino C++ and built using PlatformIO.

Firmware Features

Deep sleep power management

Sensor abstraction and modular drivers

SD card file logging

Optional Wi-Fi web server

Non-blocking design principles

Build Instructions
cd firmware
pio run
pio run -t upload

Platform

Framework: Arduino

Board: LoLin NodeMCU (ESP8266)

Toolchain: PlatformIO

📂 See firmware/README.md
 for firmware-specific details.

Repository Structure
ESP8266-Environmental-Data-Logger/
├── README.md
├── firmware/
│   ├── platformio.ini
│   ├── src/
│   ├── include/
│   └── README.md
├── hardware/
│   └── wiring_schematic.pdf
└── images/

Applications

This design can be adapted for:

Environmental monitoring

Agricultural soil sensing

Remote sensor nodes

Data logging prototypes

IoT proof-of-concept systems

What I Can Build for You

Based on this project, I can help with:

Custom ESP8266 / ESP32 firmware

Low-power IoT designs

Sensor integration & data logging

KiCad schematic & PCB design

Prototype-to-production transition (JLCPCB-ready files)

Author

Nick Christodoulou
Electronics & Firmware Engineer

📧 Email: nichrist71@gmail.com

🐙 GitHub: https://github.com/nichrist

License

This project is provided for demonstration and educational purposes.
Contact me for commercial use or custom development.
