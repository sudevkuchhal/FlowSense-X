# FlowSense-X Hardware

## 1. Overview

FlowSense-X is an ESP32-based intelligent motion-control system for controlling a BLDC motor through an ESC while monitoring electrical telemetry and providing local and Wi-Fi-based user interfaces.

## 2. Main Controller

| Component | Specification |
|---|---|
| Microcontroller | ESP32 DevKit V1 / ESP-WROOM-32 |
| Framework | Arduino |
| Platform | PlatformIO |
| Logic level | 3.3 V |

## 3. Motor and ESC

| Component | Specification |
|---|---|
| BLDC motor | A2212 1000KV |
| ESC | 30A ESC |
| ESC control | PWM |
| ESC signal GPIO | GPIO18 |

## 4. Sensors

### Current Sensor

- Sensor: ACS712-20A
- ADC input: GPIO34
- Voltage divider: 10k / 20k

### Voltage Sensor

- Used for battery/system voltage monitoring
- ADC input: GPIO35

### IR Optical RPM Sensor

The final physical hardware configuration uses an IR reflective sensor for physical rotational-speed measurement.

The IR sensor is positioned toward a black reference mark on the rotating motor assembly. Each time the black mark passes the sensor, one detection pulse is generated.

- Sensor type: IR reflective sensor
- Sensor input: GPIO33
- Detection target: Black reference mark on rotating motor
- Pulses per revolution: 1
- Measurement method: Physical pulse counting
- RPM source: IR sensor feedback
- ESC PWM-based RPM estimation: Not used

#### Measurement Principle

```text
Motor rotation
      ↓
Black reference mark
      ↓
IR sensor detection
      ↓
1 pulse
      ↓
Pulse timing / counting
      ↓
Measured RPM