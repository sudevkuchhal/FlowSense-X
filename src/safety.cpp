#include "safety.h"

#include "constants.h"

// ==========================================================
// FlowSense-X
// M12 Safety Manager
// ==========================================================
//
// Safety hierarchy:
//
// 1. Emergency Stop
// 2. Sensor Fault
// 3. Invalid Command
// 4. Over-Speed
// 5. Over-Current
// 6. Warning
// 7. Safe
//
// ==========================================================

namespace
{
    // ------------------------------------------------------
    // Safety thresholds
    // ------------------------------------------------------

    constexpr float CURRENT_WARNING_THRESHOLD_A =
        CURRENT_WARNING_A;

    constexpr float CURRENT_FAULT_THRESHOLD_A =
        CURRENT_LIMIT_A;

    constexpr float RPM_WARNING_MARGIN =
        300.0f;

    // ------------------------------------------------------
    // Current safety status
    // ------------------------------------------------------

    SafetyStatus status{
        SafetyState::Safe,
        SafetyFault::None,
        true,
        true
    };
}

// ==========================================================
// INITIALIZATION
// ==========================================================

void safetyInit()
{
    status.state =
        SafetyState::Safe;

    status.fault =
        SafetyFault::None;

    status.throttleAllowed =
        true;

    status.motorAllowed =
        true;
}

// ==========================================================
// SAFETY UPDATE
// ==========================================================

void safetyUpdate(
    const SafetyInput& input
)
{
    // ======================================================
    // 1. EMERGENCY STOP
    // ======================================================

    if (input.emergencyRequest)
    {
        status.state =
            SafetyState::EmergencyStop;

        status.fault =
            SafetyFault::EmergencyRequest;

        status.throttleAllowed =
            false;

        status.motorAllowed =
            false;

        return;
    }

    // ======================================================
    // 2. SENSOR VALIDATION
    // ======================================================
    //
    // IMPORTANT:
    //
    // rpmSensorValid == true
    // does NOT mean RPM must be > 0.
    //
    // A stopped motor can legitimately report:
    //
    //     RPM = 0
    //
    // ======================================================

    if (
        input.targetRpm > MOTOR_MIN_RPM &&
        (!input.currentSensorValid || !input.rpmSensorValid)
    )
    {
        status.state =
            SafetyState::Fault;

        status.fault =
            SafetyFault::SensorFault;

        status.throttleAllowed =
            false;

        status.motorAllowed =
            false;

        return;
    }

    // ======================================================
    // 3. TARGET VALIDATION
    // ======================================================

    if (
        input.targetRpm < 0.0f ||
        input.targetRpm > MAX_SAFE_RPM
    )
    {
        status.state =
            SafetyState::Fault;

        status.fault =
            SafetyFault::InvalidCommand;

        status.throttleAllowed =
            false;

        status.motorAllowed =
            false;

        return;
    }

    // ======================================================
    // 4. OVERSPEED PROTECTION
    // ======================================================

    if (
        input.rpm >= MAX_SAFE_RPM
    )
    {
        status.state =
            SafetyState::Fault;

        status.fault =
            SafetyFault::OverSpeed;

        status.throttleAllowed =
            false;

        status.motorAllowed =
            false;

        return;
    }

    // ======================================================
    // 5. OVER-CURRENT PROTECTION
    // ======================================================

    if (
        input.currentA >=
        CURRENT_FAULT_THRESHOLD_A
    )
    {
        status.state =
            SafetyState::Fault;

        status.fault =
            SafetyFault::OverCurrent;

        status.throttleAllowed =
            false;

        status.motorAllowed =
            false;

        return;
    }

    // ======================================================
    // 6. WARNING
    // ======================================================

    const bool highCurrent =
        input.currentA >=
        CURRENT_WARNING_THRESHOLD_A;

    const bool nearOverspeed =
        input.rpm >=
        (
            MAX_SAFE_RPM -
            RPM_WARNING_MARGIN
        );

    if (
        highCurrent ||
        nearOverspeed
    )
    {
        status.state =
            SafetyState::Warning;

        status.fault =
            SafetyFault::None;

        status.throttleAllowed =
            true;

        status.motorAllowed =
            true;

        return;
    }

    // ======================================================
    // 7. NORMAL
    // ======================================================

    status.state =
        SafetyState::Safe;

    status.fault =
        SafetyFault::None;

    status.throttleAllowed =
        true;

    status.motorAllowed =
        true;
}

// ==========================================================
// GET STATUS
// ==========================================================

SafetyStatus safetyGetStatus()
{
    return status;
}

// ==========================================================
// CLEAR FAULT
// ==========================================================

void safetyClearFault()
{
    status.state =
        SafetyState::Safe;

    status.fault =
        SafetyFault::None;

    status.throttleAllowed =
        true;

    status.motorAllowed =
        true;
}

// ==========================================================
// MOTOR PERMISSION
// ==========================================================

bool safetyIsMotorAllowed()
{
    return status.motorAllowed;
}

// ==========================================================
// THROTTLE PERMISSION
// ==========================================================

bool safetyIsThrottleAllowed()
{
    return status.throttleAllowed;
}