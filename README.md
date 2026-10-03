# FlowSense-X

## ESP32-Based Intelligent Motion Quality Controller

**FlowSense-X** is an ESP32-based intelligent BLDC motor control and monitoring platform designed around an **ESC-driven A2212 1000KV BLDC motor**.

The system combines **closed-loop-ready motion control architecture, adaptive acceleration/deceleration, Auto Peak motion profiles, electrical telemetry, OLED-based local monitoring, Wi-Fi connectivity, and a browser-based real-time dashboard** into a single embedded control platform.

> **Current build note:** An RPM sensor is not installed in the present hardware configuration. Therefore, FlowSense-X does **not** claim measured motor RPM. ESC PWM is treated as a control command, not as physical RPM feedback.

---

## Key Features

### Motor & Motion Control

* ESP32-based BLDC motor controller
* ESC-based PWM motor control
* Configurable target-speed control
* Adaptive acceleration and deceleration
* Smooth motion ramping
* Auto Peak motion profile
* Configurable motion parameters
* Safety-controlled motor startup and shutdown

### Telemetry & Monitoring

* ACS712-based current monitoring
* Battery/system voltage monitoring
* Real-time electrical telemetry
* OLED telemetry display
* System state and fault information
* Runtime event diagnostics
* Dashboard telemetry history

### User Interface

* 0.96-inch SSD1306 OLED
* Physical **UP / DOWN / MODE** controls
* Wi-Fi connectivity
* Browser-based monitoring dashboard
* Real-time dashboard updates
* Dashboard data export

### Safety & Reliability

* Controlled ESC initialization
* Safety state management
* Fault/status handling
* Motor availability diagnostics
* Sensor availability diagnostics
* Controlled motion transitions

---

## System Overview

```text
                         ┌─────────────────────────┐
                         │          ESP32          │
                         │     Control Center      │
                         │                         │
                         │ Motion Control          │
                         │ Safety Management       │
                         │ Telemetry Processing    │
                         │ Wi-Fi / HTTP            │
                         └────────────┬────────────┘
                                      │
              ┌───────────────────────┼───────────────────────┐
              │                       │                       │
              ▼                       ▼                       ▼
       ┌─────────────┐        ┌──────────────┐        ┌─────────────┐
       │ 30A ESC     │        │   Sensors    │        │   OLED      │
       │ PWM Control │        │              │        │ SSD1306     │
       └──────┬──────┘        │ ACS712       │        └─────────────┘
              │               │ Voltage      │
              ▼               └──────┬───────┘
       ┌─────────────┐               │
       │ A2212 1000KV│               ▼
       │ BLDC Motor  │        ┌──────────────┐
       └─────────────┘        │  Telemetry   │
                              └──────┬───────┘
                                     │
                                     ▼
                              ┌──────────────┐
                              │ Wi-Fi / HTTP │
                              └──────┬───────┘
                                     │
                                     ▼
                           ┌────────────────────┐
                           │ Browser Dashboard  │
                           │ Live Monitoring    │
                           │ History / Export   │
                           └────────────────────┘
```

---



## Engineering Architecture & Data Flow

FlowSense-X is organized as a layered embedded control system in which
user commands, motion control, sensor telemetry, safety decisions, and
remote monitoring are processed through the ESP32 control core.

```text
                         ┌──────────────────────────┐
                         │       USER INPUT         │
                         │                          │
                         │ UP / DOWN / MODE         │
                         │ Browser Control          │
                         └────────────┬─────────────┘
                                      │
                                      ▼
                         ┌──────────────────────────┐
                         │     MOTION MANAGER       │
                         │                          │
                         │ Target Motion            │
                         │ Motion Profile           │
                         │ Auto Peak                │
                         └────────────┬─────────────┘
                                      │
                                      ▼
                         ┌──────────────────────────┐
                         │     ADAPTIVE RAMP        │
                         │                          │
                         │ Acceleration             │
                         │ Deceleration             │
                         │ Command Smoothing        │
                         └────────────┬─────────────┘
                                      │
                                      ▼
                         ┌──────────────────────────┐
                         │     SAFETY MANAGER       │
                         │                          │
                         │ Sensor Validity          │
                         │ Fault Conditions         │
                         │ Emergency Stop           │
                         └────────────┬─────────────┘
                                      │
                             Safe Command
                                      │
                                      ▼
                         ┌──────────────────────────┐
                         │       ESC DRIVER         │
                         │                          │
                         │ 50 Hz PWM                │
                         │ Pulse Width Control      │
                         └────────────┬─────────────┘
                                      │
                                      ▼
                         ┌──────────────────────────┐
                         │     30A ESC + A2212      │
                         │        BLDC MOTOR        │
                         └──────────────────────────┘


      ┌────────────────────────────────────────────────────┐
      │                 SENSOR / TELEMETRY                 │
      │                                                    │
      │   ACS712          Voltage Sensor       System      │
      │      │                  │              State       │
      │      └──────────────────┼────────────────┘         │
      │                         ▼                          │
      │                ┌──────────────────┐                │
      │                │  ESP32 Telemetry │                │
      │                │     Engine       │                │
      │                └────────┬─────────┘                │
      │                         │                          │
      └─────────────────────────┼──────────────────────────┘
                                │
                    ┌───────────┴────────────┐
                    │                        │
                    ▼                        ▼
             ┌──────────────┐       ┌─────────────────┐
             │ SSD1306 OLED │       │  Wi-Fi / HTTP   │
             │ Local Status │       │ Communication   │
             └──────────────┘       └────────┬────────┘
                                             │
                                             ▼
                                    ┌──────────────────┐
                                    │ Browser          │
                                    │ Dashboard        │
                                    │                  │
                                    │ Live Telemetry   │
                                    │ History          │
                                    │ Events           │
                                    │ Data Export      │
                                    └──────────────────┘

---


## Hardware

| Component       | Specification                  |
| --------------- | ------------------------------ |
| Microcontroller | ESP32 DevKit V1 / ESP-WROOM-32 |
| Motor           | A2212 1000KV BLDC              |
| ESC             | 30A ESC                        |
| Current Sensor  | ACS712-20A                     |
| Voltage Sensor  | Voltage sensing module         |
| Display         | SSD1306 128×64 OLED            |
| User Controls   | 3× tactile push buttons        |
| Communication   | Wi-Fi                          |
| Motor Control   | ESC PWM                        |
| Test Supply     | Approximately 12.20 V          |
| RPM Sensor      | **Not installed**              |

### GPIO Configuration

| Function       |   GPIO |
| -------------- | -----: |
| ESC Signal     | GPIO18 |
| OLED SDA       | GPIO21 |
| OLED SCL       | GPIO22 |
| Current Sensor | GPIO34 |
| Voltage Sensor | GPIO35 |
| UP Button      | GPIO25 |
| DOWN Button    | GPIO26 |
| MODE Button    | GPIO27 |

> **Important:** GPIO assignments should be treated as project configuration values and verified against the firmware before modifying the hardware wiring.

---

## Motion Control

FlowSense-X separates **motion commands** from physical telemetry.

The controller generates an ESC PWM command and applies motion-management logic before sending the command to the ESC.

```text
User Command
     │
     ▼
Target Motion
     │
     ▼
Motion Profile
     │
     ├── Adaptive Ramp
     │
     ├── Acceleration Control
     │
     └── Deceleration Control
     │
     ▼
Safety Validation
     │
     ▼
ESC PWM Command
     │
     ▼
30A ESC
     │
     ▼
A2212 BLDC Motor
```

This architecture allows the firmware to manage smooth motor transitions instead of directly applying abrupt control changes.

---

## Auto Peak

**Auto Peak** is a motion-profile mode designed to automatically manage a predefined motor command profile.

Conceptually:

```text
        Peak
         /\
        /  \
       /    \
      /      \
_____/        \_____
   Ramp       Ramp
```

The profile can be used to create controlled acceleration, peak operation, and deceleration while maintaining safety constraints.

---

## Telemetry Pipeline

FlowSense-X collects electrical and system information at the ESP32 and exposes the processed telemetry to both the local OLED interface and the browser dashboard.

```text
ACS712 ───────┐
              │
Voltage ──────┼──► ESP32 ──► Telemetry Engine ──► OLED
              │                         │
System State ─┘                         │
                                        ▼
                                  Wi-Fi / HTTP
                                        │
                                        ▼
                              Browser Dashboard
```

---

## RPM Measurement

### Current Status

The current FlowSense-X hardware **does not include an RPM sensor**.

Therefore:

* Physical motor RPM is **not measured**.
* ESC PWM is **not reported as measured RPM**.
* PWM percentage is a control parameter, not an RPM measurement.
* Actual RPM feedback can be added later using a dedicated RPM sensing mechanism.

This distinction keeps the telemetry scientifically valid and prevents estimated control values from being presented as sensor measurements.

---

## OLED Interface

The SSD1306 OLED provides local system information without requiring a computer.

Typical information includes:

```text
┌─────────────────────────┐
│     FLOWSENSE-X         │
├─────────────────────────┤
│ MODE: AUTO PEAK         │
│ STATE: READY            │
│ PWM: 1200 us            │
│ VOLT: XX.XX V           │
│ CURR: XX.XX A           │
├─────────────────────────┤
│ UP/DN: CONTROL          │
│ MODE: SELECT            │
└─────────────────────────┘
```

The exact displayed fields depend on the active firmware configuration.

---

## Browser Dashboard

FlowSense-X provides a Wi-Fi-connected browser dashboard for remote monitoring.

### Dashboard Capabilities

* Live telemetry
* Motor/control status
* Voltage monitoring
* Current monitoring
* Motion profile information
* System diagnostics
* Event/history information
* Data export

```text
ESP32
  │
  │ Wi-Fi
  ▼
HTTP/API Layer
  │
  ▼
Web Dashboard
  │
  ├── Live Telemetry
  ├── System Status
  ├── History
  └── Data Export
```

---

## Safety Architecture

Safety is treated as a separate layer from the motion-control logic.

```text
             Motion Command
                   │
                   ▼
          ┌─────────────────┐
          │ Safety Manager  │
          └────────┬────────┘
                   │
          ┌────────┴────────┐
          │                 │
       SAFE              FAULT
          │                 │
          ▼                 ▼
     Allow Motion       Stop/Restrict
```

The firmware can use system and sensor status to determine whether motor operation should be permitted.

---

## Software Stack

| Layer         | Technology |
| ------------- | ---------- |
| MCU           | ESP32      |
| Framework     | Arduino    |
| Build System  | PlatformIO |
| Motor Control | ESC PWM    |
| Display       | SSD1306    |
| Communication | Wi-Fi      |
| Dashboard     | Web / HTTP |
| Storage       | LittleFS   |
| Language      | C/C++      |

---

## Project Structure

```text
FlowSense-X/
│
├── src/
│   ├── main.cpp
│   └── ...
│
├── include/
│   └── ...
│
├── lib/
│   └── ...
│
├── data/
│   └── dashboard/
│
├── test/
│   └── ...
│
├── platformio.ini
├── README.md
├── LICENSE
└── .gitignore
```

> The exact source-file structure may change as the firmware and dashboard evolve.

---

## Build & Upload

### 1. Clone Repository

```bash
git clone https://github.com/sudevkuchhal/FlowSense-X.git
cd FlowSense-X
```

### 2. Build Firmware

```bash
pio run
```

If PlatformIO CLI is not available in PATH on Windows:

```powershell
D:\.platformio\penv\Scripts\platformio.exe run
```

### 3. Upload to ESP32

```powershell
D:\.platformio\penv\Scripts\platformio.exe run --target upload
```

### 4. Serial Monitor

```powershell
D:\.platformio\penv\Scripts\platformio.exe device monitor -b 115200
```

---

## Development Workflow

FlowSense-X is developed around a hardware-first embedded workflow:

```text
Hardware
   │
   ▼
GPIO / Sensor Verification
   │
   ▼
Motor Control
   │
   ▼
Safety Layer
   │
   ▼
Telemetry
   │
   ▼
OLED Interface
   │
   ▼
Wi-Fi / API
   │
   ▼
Browser Dashboard
   │
   ▼
Testing & Validation
```

---

## Validation Status

| Subsystem               | Status                             |
| ----------------------- | ---------------------------------- |
| ESP32 Firmware          | Implemented                        |
| ESC PWM Control         | Implemented                        |
| Adaptive Motion Control | Implemented                        |
| Auto Peak Profile       | Implemented                        |
| OLED Interface          | Implemented                        |
| Current Monitoring      | Implemented                        |
| Voltage Monitoring      | Implemented                        |
| Wi-Fi Connectivity      | Implemented                        |
| Web Dashboard           | Implemented                        |
| Safety Handling         | Implemented                        |
| RPM Sensor              | **installed**                      |
| Physical RPM Feedback   | **available in current build**     |

---

## Design Principle

FlowSense-X is designed around a simple principle:

> **Control the motor intelligently, monitor the electrical system continuously, and present only telemetry that the hardware can actually measure.**

This makes the system suitable as a foundation for further development in:

* Embedded Systems
* BLDC Motor Control
* Edge AI / Intelligent Control
* IoT Monitoring
* Industrial Automation
* Motion Quality Analysis
* Embedded Web Interfaces

---

## Future Development

Potential future extensions include:

* Dedicated RPM sensing
* True closed-loop RPM control
* Advanced PID tuning
* More motion profiles
* Sensor calibration tools
* Extended fault detection
* OTA firmware updates
* Advanced analytics
* Machine-learning-assisted motion quality analysis
* Expanded dashboard visualization

---

## License

This project is released under the **MIT License**.

See [`LICENSE`](LICENSE) for details.

---

## Author

**Sudev Kuchhal**

Electronics & Computer Engineering
Embedded Systems • Edge AI • IoT • Intelligent Control

GitHub: **[@sudevkuchhal](https://github.com/sudevkuchhal)**

---

## Project

**FlowSense-X — ESP32-Based Intelligent Motion Quality Controller**

Designed and developed as an embedded motor-control and telemetry platform combining **real-time control, electrical sensing, local visualization, wireless monitoring, and intelligent motion profiles**.
