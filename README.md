# FlowSense-X

## ESP32-Based Intelligent Motion Quality Controller

FlowSense-X is an ESP32-based BLDC motor control and monitoring system designed around an ESC-driven A2212 1000KV motor.

The system combines firmware-based motor control, adaptive motion control, electrical telemetry, OLED monitoring, Wi-Fi connectivity, a browser dashboard, safety handling, and Auto Peak motion profiles.

---

## Features

- ESP32-based BLDC/ESC control
- PWM-based ESC control
- Adaptive ramp control
- Auto Peak motion profile
- Target-speed control
- ACS712-based current monitoring
- Battery/system voltage monitoring
- SSD1306 OLED telemetry
- UP / DOWN / MODE physical controls
- Wi-Fi connectivity
- Browser-based dashboard
- Real-time telemetry
- Safety and fault handling
- Event diagnostics
- API endpoints for dashboard communication
- Dashboard history and data export

---

## Current Hardware

| Component | Specification |
|---|---|
| Controller | ESP32 DevKit V1 / ESP-WROOM-32 |
| Motor | A2212 1000KV BLDC |
| ESC | 30A ESC |
| Current Sensor | ACS712-20A |
| Voltage Sensor | Voltage sensor |
| Display | SSD1306 128×64 OLED |
| Controls | 3× tactile buttons |
| Test Supply | Approximately 12.20 V |
| RPM Sensor | **Not installed in current build** |

### Pin Configuration

| Function | GPIO |
|---|---:|
| ESC Signal | GPIO18 |
| OLED SDA | GPIO21 |
| OLED SCL | GPIO22 |
| Current Sensor | GPIO34 |
| Voltage Sensor | GPIO35 |
| UP Button | GPIO25 |
| DOWN Button | GPIO26 |
| MODE Button | GPIO27 |

> **RPM note:** The current physical build does not contain an RPM sensor. ESC PWM is not treated as measured RPM. Physical RPM feedback requires an actual RPM sensor.

---

## System Architecture

```text
                 ┌─────────────────────┐
                 │      ESP32          │
                 │   Control Center    │
                 └──────────┬──────────┘
                            │
          ┌─────────────────┼─────────────────┐
          │                 │                 │
          ▼                 ▼                 ▼
     ESC / BLDC        Sensors           OLED
      GPIO18        Current/Voltage     SSD1306
          │                 │
          ▼                 ▼
      A2212 Motor       Telemetry
                            │
                            ▼
                       Wi-Fi / HTTP
                            │
                            ▼
                    Browser Dashboard