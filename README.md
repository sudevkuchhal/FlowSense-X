# FlowSense-X Control Center — Enhanced Build

This build keeps the existing ESP32/ESC/current/voltage/OLED architecture and adds a premium browser dashboard plus firmware-side telemetry and Auto Peak control.

## Hardware
- ESP32 DevKit V1
- A2212 1000KV BLDC + ESC
- IR reflective RPM sensor on GPIO33, one black marker/revolution
- ACS712-20A on GPIO34
- 0–25 V voltage sensor on GPIO35
- ESC signal GPIO18
- SSD1306 OLED: SDA 21 / SCL 22

## Dashboard
The dashboard is embedded in firmware, so use a normal PlatformIO firmware upload. **Do not use `uploadfs` for this dashboard.**

Features:
- Live measured/target/command RPM
- Voltage, current and calculated electrical power
- ESC pulse telemetry
- IR sensor state, pulse count and pulses/second
- Manual absolute-RPM control
- Smooth-flow factor 0.25×–1.50×
- Auto Peak state machine: RAMP UP → PEAK APPROACH → PEAK HOLD → RAMP DOWN → STOP → IDLE
- Configurable peak, series step, ramp-up, hold and ramp-down
- Real-time canvas chart
- Browser history, minute summaries and CSV/JSON export
- Safety/event diagnostics
- Firmware/network/heap/RSSI information
- API contract and pin map

## Firmware API
- `GET /api/status`
- `GET /api/rpm?value=0-14000`
- `GET /api/speed?value=0-100`
- `GET /api/start`
- `GET /api/stop`
- `GET /api/emergency?value=1`
- `GET /api/auto/start`
- `GET /api/auto/stop`
- `GET /api/config?peak=&step=&hold=&up=&down=&smooth=`
- `GET /api/info`

## RPM measurement
RPM is **not** estimated from ESC PWM. The IR sensor interrupt records pulse-to-pulse period and calculates:

`RPM = 60,000,000 / pulse_period_us / pulses_per_revolution`

A 500 ms signal timeout drives measured RPM to zero.

## Upload
From the directory containing `platformio.ini`:

```powershell
D:\.platformio\penv\Scripts\platformio.exe run
D:\.platformio\penv\Scripts\platformio.exe run --target upload --upload-port COM10
D:\.platformio\penv\Scripts\platformio.exe device monitor --port COM10 --baud 115200
```

Open the IP printed by the ESP32 in a browser.

## Safety
The dashboard does not bypass firmware safety. Target RPM is clamped by `MAX_SAFE_RPM` (14,000 RPM in this source build), current protection remains active, and the final ESC pulse remains clamped to 1000–2000 µs.
