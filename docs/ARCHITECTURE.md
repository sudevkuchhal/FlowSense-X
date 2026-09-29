# FlowSense-X Architecture

## 1. Overview

FlowSense-X is an ESP32-based intelligent motion control system designed to control a BLDC motor through an ESC while monitoring electrical and motion-related telemetry.

The firmware combines:

- BLDC/ESC control
- Adaptive ramp control
- Auto Peak motion profiles
- Current sensing
- Voltage sensing
- Physical IR-based RPM feedback
- Safety and fault handling
- OLED telemetry
- Wi-Fi connectivity
- HTTP API
- Browser-based dashboard

---

## 2. System Architecture

```text
                    ┌──────────────────────┐
                    │     User / Operator  │
                    └──────────┬───────────┘
                               │
                               ▼
                    ┌──────────────────────┐
                    │ Web Dashboard        │
                    │ Live / Auto / Events │
                    └──────────┬───────────┘
                               │ HTTP
                               ▼
                    ┌──────────────────────┐
                    │ ESP32 HTTP API       │
                    │ web_dashboard.cpp    │
                    └──────────┬───────────┘
                               │
              ┌────────────────┼────────────────┐
              │                │                │
              ▼                ▼                ▼
       Motion Control      Telemetry         Safety
              │                │                │
              ▼                ▼                ▼
       Adaptive Ramp       Sensors         Events/Faults
              │                │                │
              └───────────────┼────────────────┘
                              ▼
                    ┌──────────────────────┐
                    │ ESC / Motor Output   │
                    │ GPIO18 / PWM         │
                    └──────────────────────┘