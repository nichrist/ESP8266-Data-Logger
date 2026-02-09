# ESP8266 Environmental Data Logger – Firmware

Firmware for a dual-mode ESP8266 environmental data logger.

## Features
- Deep sleep power management
- BMP280 + RTC via I²C
- Soil moisture analog sensing
- SD card logging via SPI
- Optional Wi-Fi access point with web UI

## Build
This project uses PlatformIO.

```bash
pio run
pio run -t upload
