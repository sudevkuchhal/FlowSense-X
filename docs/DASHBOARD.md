# FlowSense-X Dashboard

## 1. Overview

FlowSense-X provides a browser-based dashboard for monitoring and controlling the ESP32 motion-control system over Wi-Fi.

The dashboard is designed to provide:

- Motor control
- Speed/target control
- Live telemetry
- Auto Peak control
- Safety status
- Event monitoring
- System information
- Data export
- API-based communication with the ESP32

The dashboard communicates with the ESP32 through HTTP endpoints exposed by the firmware.

## 2. Dashboard Architecture

```text
┌──────────────────────────────┐
│        Web Browser           │
│  FlowSense-X Dashboard       │
└──────────────┬───────────────┘
               │ HTTP
               ▼
┌──────────────────────────────┐
│          ESP32               │
│     FlowSense-X Firmware     │
├──────────────────────────────┤
│ HTTP API                     │
│ Motion Control               │
│ Safety System                │
│ Telemetry                    │
└──────────────┬───────────────┘
               │
       ┌───────┴────────┐
       ▼                ▼
    ESC / BLDC       Sensors
                     ├─ Current
                     ├─ Voltage
                     └─ RPM*
```

`*` RPM feedback is only physically valid when the corresponding RPM sensor is installed and producing valid measurements.

## 3. Dashboard Files

```text
dashboard/
├── flowsense-dashboard.html
├── FlowSense-X-Premium.html
├── flowsense-desktop-dashboard.html
└── README.md
```

### `flowsense-dashboard.html`

Basic browser dashboard implementation.

### `FlowSense-X-Premium.html`

Feature-rich FlowSense-X dashboard containing:

- Auto Peak controls
- Live telemetry
- Safety information
- Charts
- Event monitoring
- API controls
- Data export
- System information

### `flowsense-desktop-dashboard.html`

Desktop-oriented dashboard implementation with:

- ESP32 IP configuration
- Live mode
- Demo mode
- Start/Stop controls
- Speed control
- Telemetry
- Event logging
- Charts
- JSON export
- CSV export
- HTTP API helpers

## 4. Connecting to the ESP32

The ESP32 must be connected to the same network as the computer running the dashboard.

Example:

```text
ESP32 IP:
10.200.23.184
```

The IP address may change depending on the Wi-Fi network and DHCP configuration.

Enter the current ESP32 IP address in the dashboard before connecting.

Example:

```text
http://10.200.23.184
```

## 5. Live Mode

Live mode communicates directly with the ESP32.

```text
Dashboard
    │
    │ HTTP Request
    ▼
ESP32 API
    │
    ▼
Firmware
    │
    ├── Motor Control
    ├── Sensors
    └── Safety
    │
    ▼
JSON Response
    │
    ▼
Dashboard
```

Live mode should only be used when the ESP32 firmware and dashboard API are compatible.

## 6. Demo Mode

Demo mode allows the dashboard interface to be evaluated without an active ESP32 connection.

Demo mode can provide simulated:

- Telemetry
- Motor state
- Speed values
- Charts
- Events
- System status

Demo values must not be interpreted as physical measurements.

## 7. Main Dashboard Sections

### Motor Control

The dashboard provides controls for:

- Start
- Stop
- Emergency stop
- Speed/target adjustment
- Auto Peak start
- Auto Peak stop

Commands are sent to the ESP32 through the HTTP API.

### Live Telemetry

The dashboard can display telemetry such as:

| Parameter | Description |
|---|---|
| Target RPM | Requested motor speed |
| Measured RPM | Physical RPM feedback when valid |
| Speed | Current control value |
| Current | Electrical current telemetry |
| Voltage | Supply/battery voltage |
| Motor State | Current motor state |
| Safety State | Current safety state |
| Wi-Fi State | Network connection state |

The dashboard must distinguish between measured sensor data and simulated/demo data.

## 8. Auto Peak

The Premium dashboard provides Auto Peak configuration and control.

Typical parameters include:

| Parameter | Description |
|---|---|
| Ramp Up | Time used to increase speed |
| Peak Target | Desired peak target |
| Peak Hold | Time to hold the peak |
| Ramp Down | Time used to decrease speed |

Example dashboard defaults:

```text
Ramp Up:    12 seconds
Peak Hold:   3 seconds
Ramp Down:   8 seconds
```

The firmware remains responsible for enforcing actual control and safety limits.

## 9. Auto Peak State Flow

```text
IDLE
  │
  ▼
RAMP UP
  │
  ▼
PEAK APPROACH
  │
  ▼
PEAK HOLD
  │
  ▼
RAMP DOWN
  │
  ▼
STOP
  │
  ▼
IDLE
```

If a safety fault or emergency request occurs, the normal Auto Peak sequence must be interrupted according to the firmware safety implementation.

## 10. Safety Information

The dashboard exposes safety-related information to the operator.

Relevant conditions may include:

- Current sensor status
- RPM sensor status
- Voltage sensor status
- Motor availability
- Command validity
- Emergency stop state
- Overspeed protection
- Sensor faults

The dashboard is an interface to the safety system. It does not replace firmware-level safety enforcement.

## 11. RPM Measurement

FlowSense-X does not estimate physical RPM from ESC PWM.

When an RPM sensor is installed and operating correctly:

```text
IR Sensor
    │
    ▼
ESP32 RPM Input
    │
    ▼
RPM Processing
    │
    ▼
HTTP API
    │
    ▼
Dashboard
```

If the physical RPM sensor is not installed:

- RPM must not be presented as measured physical RPM.
- PWM percentage must not be converted into fake RPM.
- Demo RPM values must be clearly treated as simulated data.

## 12. API Communication

The dashboard uses HTTP API routes exposed by the ESP32 firmware.

Primary routes include:

| Endpoint | Purpose |
|---|---|
| `/api/status` | System status and telemetry |
| `/api/rpm` | RPM-related control/data |
| `/api/speed` | Speed control |
| `/api/start` | Start motor |
| `/api/stop` | Stop motor |
| `/api/emergency` | Emergency control |
| `/api/auto/start` | Start Auto Peak |
| `/api/auto/stop` | Stop Auto Peak |
| `/api/config` | Configuration |
| `/api/info` | System information |

See [`API.md`](API.md) for the detailed API reference.

## 13. CORS

When the dashboard is served from a different origin than the ESP32, browser CORS restrictions may prevent API requests.

The firmware can provide:

```http
Access-Control-Allow-Origin: *
```

for supported API handlers.

This is useful when the dashboard HTML is opened locally or hosted separately from the ESP32.

## 14. Charts and History

The dashboard can visualize telemetry over time.

Typical chart data includes:

- Speed
- RPM
- Current
- Voltage
- Target values

Historical dashboard data may be stored locally in the browser.

Local browser history is not equivalent to persistent storage on the ESP32.

## 15. Event Logging

Dashboard events may include:

```text
SYSTEM START
ESP32 CONNECTED
MOTOR START
MOTOR STOP
AUTO PEAK START
AUTO PEAK COMPLETE
EMERGENCY STOP
SENSOR FAULT
CONNECTION LOST
```

Event logs help with development and troubleshooting.

## 16. Data Export

The dashboard supports exporting collected dashboard data where implemented.

Typical formats:

```text
CSV
JSON
```

Exported data should be treated according to its source:

- Live data = received from ESP32
- Demo data = simulated dashboard data
- Historical data = browser-stored dashboard data

## 17. Network Requirements

For live operation:

1. ESP32 must have Wi-Fi enabled.
2. ESP32 must successfully connect to the configured network.
3. The computer must be connected to the same reachable network.
4. The ESP32 IP address must be known.
5. Required HTTP API routes must be available.
6. Browser requests must not be blocked by CORS configuration.

## 18. Troubleshooting Dashboard Connection

### Dashboard cannot connect

Check:

```text
ESP32 powered
        ↓
Wi-Fi connected
        ↓
ESP32 IP available
        ↓
Computer on same network
        ↓
Correct IP entered
        ↓
API responding
```

### ESP32 IP changed

Check the serial monitor for the latest IP address.

Example:

```text
WiFi connected
IP address: 10.200.23.184
```

Use the newly reported address in the dashboard.

### API request blocked by browser

Check:

- ESP32 API route
- CORS headers
- Browser console
- Network connectivity
- Correct IP address

### Dashboard shows unavailable sensors

The dashboard should reflect the actual firmware sensor state.

Example:

```text
Motor: UNAVAILABLE
Current Sensor: UNAVAILABLE
Voltage Sensor: AVAILABLE
Wi-Fi: CONNECTED
```

This indicates that the dashboard is receiving firmware status rather than assuming every hardware component is available.

## 19. Dashboard Safety Rules

The dashboard should never be considered the primary safety mechanism.

Safety-critical decisions must remain inside the ESP32 firmware.

Before motor operation:

- Verify hardware connections.
- Verify motor/ESC configuration.
- Verify sensor availability required by the firmware.
- Keep the motor mechanically clear.
- Maintain an emergency power-disconnect method.
- Never rely on dashboard values as a substitute for physical safety checks.

## 20. Development Workflow

Recommended workflow:

```text
1. Build firmware
        ↓
2. Upload ESP32
        ↓
3. Open Serial Monitor
        ↓
4. Confirm Wi-Fi connection
        ↓
5. Note ESP32 IP
        ↓
6. Open dashboard
        ↓
7. Enter ESP32 IP
        ↓
8. Test /api/status
        ↓
9. Verify telemetry
        ↓
10. Test controls at safe conditions
```

## 21. Browser Compatibility

The dashboard uses standard browser technologies including:

- HTML
- CSS
- JavaScript
- Fetch API
- Browser local storage
- Canvas/chart rendering where implemented

A modern Chromium-, Firefox-, or Safari-based browser is recommended.

## 22. Current Implementation Notes

The repository contains multiple dashboard variants because the project has evolved through development stages.

Before deployment, ensure that:

- Dashboard API routes match the firmware.
- Sensor availability matches the physical hardware.
- RPM is not represented as measured when the RPM sensor is absent.
- Demo data is clearly distinguishable from live data.
- Safety decisions remain firmware-controlled.
