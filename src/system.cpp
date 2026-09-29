#include "system.h"

#include <Arduino.h>

#include "adaptive_ramp.h"
#include "constants.h"
#include "rpm_sensor.h"
#include "events.h"
#include "current_sensor.h"
#include "motion.h"
#include "safety.h"

// ==========================================================
// FlowSense-X
// System Core Controller
//
// Control Pipeline:
//
// User Input
//    |
//    v
// Target RPM
//    |
//    v
// Motion Profile
//    |
//    v
// Adaptive Ramp
//    |
//    v
// Safety Manager
//    |
//    v
// Event Manager
//    |
//    v
// ESC PWM
//
// RPM feedback:
// IR RPM Sensor -> systemSetMeasuredRpm()
// ==========================================================

namespace
{
    // ------------------------------------------------------
    // Runtime command state
    // ------------------------------------------------------

    float targetRpm   = 0.0f;
    float commandRpm  = 0.0f;
    float measuredRpm = 0.0f;

    // ------------------------------------------------------
    // Electrical telemetry
    // ------------------------------------------------------

    float currentA = 0.0f;
    float voltageV = 0.0f;

    // ------------------------------------------------------
    // Emergency request
    // ------------------------------------------------------

    bool emergencyRequest = false;

    // ------------------------------------------------------
    // System status
    // ------------------------------------------------------

    SystemStatus status{
        SystemState::Idle,

        0.0f, // targetRpm
        0.0f, // commandRpm
        0.0f, // measuredRpm

        0.0f, // currentA
        0.0f, // voltageV

        ESC_MIN_US, // requestedEscPulseUs
        ESC_MIN_US, // escPulseUs

        true, // safetyAllowed

        FaultCode::None,
        EventType::None
    };

    // ------------------------------------------------------
    // Motion / ramp configuration
    // ------------------------------------------------------

    constexpr float BASE_RAMP_RPM_PER_SEC = 800.0f;

    // ------------------------------------------------------
    // Helpers
    // ------------------------------------------------------

    float rpmToEscPulse(float rpm)
    {
        if (MAX_SAFE_RPM <= 0.0f)
        {
            return static_cast<float>(ESC_MIN_US);
        }

        const float clampedRpm =
            constrain(
                rpm,
                0.0f,
                MAX_SAFE_RPM
            );

        const float percentage =
            (
                clampedRpm /
                MAX_SAFE_RPM
            ) * 100.0f;

        const float escRange =
            static_cast<float>(
                ESC_MAX_US - ESC_MIN_US
            );

        return
            static_cast<float>(ESC_MIN_US) +
            (
                percentage / 100.0f
            ) * escRange;
    }

    SystemState determineSystemState(
        const SafetyStatus& safety,
        const EventStatus& event
    )
    {
        if (
            safety.state ==
            SafetyState::EmergencyStop
        )
        {
            return SystemState::EmergencyStop;
        }

        if (
            safety.state ==
            SafetyState::Fault
        )
        {
            return SystemState::Fault;
        }

        if (
            safety.state ==
            SafetyState::Warning
        )
        {
            return SystemState::Warning;
        }

        if (
            event.type ==
            EventType::Emergency
        )
        {
            return SystemState::EmergencyStop;
        }

        if (
            event.type ==
            EventType::Fault
        )
        {
            return SystemState::Fault;
        }

        if (
            event.type ==
            EventType::Warning
        )
        {
            return SystemState::Warning;
        }

        if (targetRpm > 0.0f)
        {
            return SystemState::Running;
        }

        return SystemState::Idle;
    }
}

// ==========================================================
// Initialization
// ==========================================================

void systemInit()
{
    targetRpm = 0.0f;
    commandRpm = 0.0f;
    measuredRpm = 0.0f;

    currentA = 0.0f;
    voltageV = 0.0f;

    emergencyRequest = false;

    motionInit();

    adaptiveRampInit(
        BASE_RAMP_RPM_PER_SEC
    );

    safetyInit();
    eventsInit();

    status = {
        SystemState::Idle,

        0.0f,
        0.0f,
        0.0f,

        0.0f,
        0.0f,

        ESC_MIN_US,
        ESC_MIN_US,

        true,

        FaultCode::None,
        EventType::None
    };
}

// ==========================================================
// Target RPM
// ==========================================================

void systemSetTargetRpm(
    float newTargetRpm
)
{
    targetRpm =
        constrain(
            newTargetRpm,
            0.0f,
            MAX_SAFE_RPM
        );

    motionSetTarget(
        targetRpm
    );

    adaptiveRampSetTarget(
        targetRpm
    );

    if (targetRpm > 0.0f)
    {
        motionStart();
    }
    else
    {
        motionStop();
        commandRpm = 0.0f;
    }
}

// ==========================================================
// Measured RPM
// ==========================================================
//
// Called by the IR RPM sensor module.
//
// IMPORTANT:
// This is measured RPM, not commanded RPM.
// ==========================================================

void systemSetMeasuredRpm(
    float rpm
)
{
    measuredRpm =
        constrain(
            rpm,
            0.0f,
            MAX_SAFE_RPM * 1.5f
        );
}

// ==========================================================
// Current
// ==========================================================

void systemSetCurrent(
    float current
)
{
    currentA =
        max(
            0.0f,
            current
        );
}

// ==========================================================
// Voltage
// ==========================================================

void systemSetVoltage(
    float voltage
)
{
    voltageV =
        max(
            0.0f,
            voltage
        );
}

// ==========================================================
// Actual ESC Pulse
// ==========================================================
//
// Keeps software telemetry synchronized with the pulse
// actually written to the ESC.
// ==========================================================

void systemSetActualEscPulse(
    uint16_t pulseUs
)
{
    status.escPulseUs =
        static_cast<uint16_t>(
            constrain(
                pulseUs,
                ESC_MIN_US,
                ESC_MAX_US
            )
        );
}

// ==========================================================
// Emergency Stop Request
// ==========================================================

void systemRequestEmergencyStop(
    bool request
)
{
    emergencyRequest = request;

    if (request)
    {
        targetRpm = 0.0f;
        commandRpm = 0.0f;

        motionStop();
        adaptiveRampSetTarget(0.0f);
    }
}

// ==========================================================
// Main System Update
// ==========================================================

void systemUpdate(
    float dtSeconds
)
{
    if (dtSeconds <= 0.0f)
    {
        return;
    }

    // ======================================================
    // 1. Update motion profile
    // ======================================================

    motionUpdate(
        dtSeconds
    );

    const MotionState motion =
        motionGetState();

    // ======================================================
    // 2. Update adaptive ramp
    // ======================================================

    adaptiveRampSetTarget(
        motion.targetRpm
    );

    adaptiveRampUpdate(
        measuredRpm,
        currentA
    );

    commandRpm =
        adaptiveRampGetCommandRpm(
            commandRpm,
            dtSeconds
        );

    // ======================================================
    // 3. Clamp command
    // ======================================================

    commandRpm =
        constrain(
            commandRpm,
            0.0f,
            MAX_SAFE_RPM
        );

    // ======================================================
    // 4. Safety evaluation
    // ======================================================

    SafetyInput safetyInput{
        currentA,
        measuredRpm,

        currentSensorIsValid(), // ACS712 calibrated validity

        rpmSensorIsValid(),

        targetRpm,

        emergencyRequest
    };

    safetyUpdate(
        safetyInput
    );

    const SafetyStatus safety =
        safetyGetStatus();

    // ======================================================
    // 5. Event processing
    // ======================================================

    eventsProcess(
        currentA,
        measuredRpm,
        targetRpm,

        currentSensorIsValid(),
        rpmSensorIsValid(),

        emergencyRequest
    );

    const EventStatus event =
        eventsGetStatus();

    // ======================================================
    // 6. Convert command RPM -> ESC PWM
    // ======================================================

    float requestedEscPulse =
        rpmToEscPulse(
            commandRpm
        );

    requestedEscPulse =
        constrain(
            requestedEscPulse,
            static_cast<float>(ESC_MIN_US),
            static_cast<float>(ESC_MAX_US)
        );

    status.requestedEscPulseUs =
        static_cast<uint16_t>(
            requestedEscPulse
        );

    // ======================================================
    // 7. Safety override
    // ======================================================

    float actualEscPulse =
        requestedEscPulse;

    const bool faultActive =
        eventsHasActiveFault();

    if (
        !safety.throttleAllowed ||
        !safety.motorAllowed ||
        faultActive ||
        emergencyRequest
    )
    {
        actualEscPulse =
            static_cast<float>(
                ESC_MIN_US
            );
    }

    // ======================================================
    // 8. Final ESC safety clamp
    // ======================================================

    actualEscPulse =
        constrain(
            actualEscPulse,
            static_cast<float>(ESC_MIN_US),
            static_cast<float>(ESC_MAX_US)
        );

    status.escPulseUs =
        static_cast<uint16_t>(
            actualEscPulse
        );

    // ======================================================
    // 9. Determine system state
    // ======================================================

    status.state =
        determineSystemState(
            safety,
            event
        );

    // ======================================================
    // 10. Publish telemetry
    // ======================================================

    status.targetRpm =
        targetRpm;

    status.commandRpm =
        commandRpm;

    status.measuredRpm =
        measuredRpm;

    status.currentA =
        currentA;

    status.voltageV =
        voltageV;

    status.safetyAllowed =
        safety.motorAllowed &&
        safety.throttleAllowed &&
        !faultActive &&
        !emergencyRequest;

    status.fault =
        event.fault;

    status.eventType =
        event.type;
}

// ==========================================================
// Get Status
// ==========================================================

SystemStatus systemGetStatus()
{
    return status;
}