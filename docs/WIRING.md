# FlowSense-X Wiring Guide

## 1. Controller

| Component | Connection |
|---|---|
| ESP32 DevKit V1 | Main controller |
| ESC signal | GPIO18 |
| OLED SDA | GPIO21 |
| OLED SCL | GPIO22 |
| ACS712 output | GPIO34 |
| Voltage sensor output | GPIO35 |
| UP button | GPIO25 |
| DOWN button | GPIO26 |
| MODE button | GPIO27 |

> RPM sensor is not part of the final physical hardware configuration.

---

## 2. OLED Display

FlowSense-X uses a 0.96-inch SSD1306 128×64 I²C OLED.

| OLED Pin | ESP32 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO21 |
| SCL | GPIO22 |

I²C address:

```text
0x3C