# Enhanced firmware changes

1. `main.cpp` now initializes and updates the physical IR RPM sensor on GPIO33.
2. `rpm_sensor.cpp` uses pulse-to-pulse timing instead of ESC PWM estimation.
3. `system.cpp` reports actual RPM-sensor validity to the safety/event layer.
4. Added `auto_peak.h/.cpp` for firmware-side Auto Peak sequencing.
5. Added configurable smooth-flow acceleration through `adaptiveRampSetBaseAcceleration()`.
6. `web_dashboard.cpp` exposes extended telemetry and Auto Peak APIs.
7. The premium dashboard is embedded, so `uploadfs` is not required.
