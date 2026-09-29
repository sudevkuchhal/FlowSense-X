#include "logger.h"

#include <Arduino.h>

#include "constants.h"

// ==========================================================
// FlowSense-X
// Runtime Data Logger
// ==========================================================
//
// Logs:
//   - Timestamp
//   - Target RPM
//   - Command RPM
//   - Measured RPM
//   - Current
//   - Voltage
//   - ESC pulse
//   - System state
//   - Safety status
//
// Output format:
//   CSV-compatible Serial data
//
// ==========================================================

namespace
{
    uint32_t lastLogMs = 0;
    bool headerPrinted = false;

    // ------------------------------------------------------
    // Convert system state to readable text
    // ------------------------------------------------------

    const char* stateToString(
        SystemState state
    )
    {
        switch (state)
        {
            case SystemState::Idle:
                return "IDLE";

            case SystemState::Running:
                return "RUN";

            case SystemState::Warning:
                return "WARN";

            case SystemState::Fault:
                return "FAULT";

            case SystemState::EmergencyStop:
                return "E_STOP";

            default:
                return "UNKNOWN";
        }
    }
}

// ==========================================================
// LOGGER INITIALIZATION
// ==========================================================

void loggerInit()
{
    lastLogMs = millis();
    headerPrinted = false;
}

// ==========================================================
// PRINT CSV HEADER
// ==========================================================

void loggerPrintHeader()
{
    Serial.println(
        "[LOG] time_ms,"
        "target_rpm,"
        "command_rpm,"
        "measured_rpm,"
        "current_a,"
        "voltage_v,"
        "esc_us,"
        "state,"
        "safety"
    );

    headerPrinted = true;
}

// ==========================================================
// LOGGER UPDATE
// ==========================================================

void loggerUpdate(
    const SystemStatus& status
)
{
    const uint32_t nowMs = millis();

    // ------------------------------------------------------
    // Logging interval
    // ------------------------------------------------------

    if (
        nowMs - lastLogMs <
        LOGGER_UPDATE_INTERVAL_MS
    )
    {
        return;
    }

    lastLogMs = nowMs;

    // ------------------------------------------------------
    // Print header once
    // ------------------------------------------------------

    if (!headerPrinted)
    {
        loggerPrintHeader();
    }

    // ------------------------------------------------------
    // Timestamp
    // ------------------------------------------------------

    Serial.print("[LOG] ");
    Serial.print(nowMs);
    Serial.print(',');

    // ------------------------------------------------------
    // Target RPM
    // ------------------------------------------------------

    Serial.print(
        status.targetRpm,
        1
    );

    Serial.print(',');

    // ------------------------------------------------------
    // Command RPM
    // ------------------------------------------------------

    Serial.print(
        status.commandRpm,
        1
    );

    Serial.print(',');

    // ------------------------------------------------------
    // IR measured RPM
    // ------------------------------------------------------

    Serial.print(
        status.measuredRpm,
        1
    );

    Serial.print(',');

    // ------------------------------------------------------
    // Current
    // ------------------------------------------------------

    Serial.print(
        status.currentA,
        3
    );

    Serial.print(',');

    // ------------------------------------------------------
    // Battery voltage
    // ------------------------------------------------------

    Serial.print(
        status.voltageV,
        3
    );

    Serial.print(',');

    // ------------------------------------------------------
    // ESC PWM pulse
    // ------------------------------------------------------

    Serial.print(
        status.escPulseUs
    );

    Serial.print(',');

    // ------------------------------------------------------
    // System state
    // ------------------------------------------------------

    Serial.print(
        stateToString(
            status.state
        )
    );

    Serial.print(',');

    // ------------------------------------------------------
    // Safety status
    // ------------------------------------------------------

    Serial.println(
        status.safetyAllowed
            ? "ALLOW"
            : "BLOCK"
    );
}