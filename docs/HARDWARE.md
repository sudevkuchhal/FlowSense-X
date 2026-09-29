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

### RPM Sensor

The final physical hardware configuration **does not include an RPM sensor**.

The firmware contains RPM-related modules because the architecture supports physical RPM feedback, but an RPM sensor is not installed in the current hardware setup.

**Important:** ESC PWM is not treated as measured RPM. RPM must only be reported as a physical measurement when an actual RPM sensor is installed and producing valid feedback.

## 5. OLED Display

| Parameter | Value |
|---|---|
| Display | 0.96-inch OLED |
| Controller | SSD1306 |
| Resolution | 128×64 |
| Interface | I2C |
| I2C address | 0x3C |
| SDA | GPIO21 |
| SCL | GPIO22 |

## 6. User Controls

| Button | GPIO |
|---|---:|
| UP | GPIO25 |
| DOWN | GPIO26 |
| MODE | GPIO27 |

## 7. Power

The documented test setup used a battery supply of approximately **12.20 V**.

The motor power path is handled through the ESC, while the ESP32 and low-voltage electronics require appropriate regulated power.

## 8. Hardware Summary

| Category | Component | Status |
|---|---|---|
| Controller | ESP32 DevKit V1 | Installed |
| Motor | A2212 1000KV BLDC | Installed |
| ESC | 30A ESC | Installed |
| Current sensing | ACS712-20A | Installed |
| Voltage sensing | Voltage sensor | Installed |
| Display | SSD1306 128×64 OLED | Installed |
| Controls | 3× tactile buttons | Installed |
| RPM feedback | Physical RPM sensor | **Not installed** |

## 9. Hardware and Firmware Notes

The firmware architecture contains RPM sensing, speed-control, and safety interfaces that can support physical RPM feedback.

However, the current physical build does not have the corresponding RPM sensor installed.

Therefore:

- Do not interpret ESC PWM as measured RPM.
- Do not claim physical RPM feedback is available on the current build.
- RPM-dependent validation requires an actual RPM sensor.
- Safety behavior requiring valid RPM feedback must be evaluated against the installed hardware configuration.

## 10. Safety Notes

- Disconnect motor power before changing wiring.
- Keep the motor mechanically secured during testing.
- Never work on exposed rotating hardware while powered.
- Verify common ground between ESP32 control electronics and ESC signal reference.
- Check sensor output voltages before connecting them to ESP32 ADC pins.
- Do not exceed voltage/current limits of the ESP32, sensors, ESC, or motor.
- Perform initial motor tests at the lowest practical control output.
- Keep the main power disconnect accessible during testing.

## 11. Related Documentation

- [Wiring](WIRING.md)
- [Architecture](ARCHITECTURE.md)
- [Firmware](FIRMWARE.md)
- [Safety](SAFETY.md)
- [Dashboard](DASHBOARD.md)
- [Validation](VALIDATION.md)
- [Troubleshooting](TROUBLESHOOTING.md)