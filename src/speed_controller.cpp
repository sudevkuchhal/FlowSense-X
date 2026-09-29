#include "speed_controller.h"

#include "constants.h"

// ==========================================================
// FlowSense-X
// M10 Closed-Loop Speed Controller
// ==========================================================
//
// Control loop:
//
//     Target RPM
//          |
//          v
//     +-----------+
//     | PID       |
//     | Controller|
//     +-----------+
//          |
//          v
//     Throttle %
//          |
//          v
//        Motor
//          |
//          v
//     IR RPM Sensor
//          |
//          +------> Measured RPM
//
// ==========================================================

namespace
{
    // ------------------------------------------------------
    // PID configuration
    // ------------------------------------------------------

    float kp = 0.0f;
    float ki = 0.0f;
    float kd = 0.0f;

    // ------------------------------------------------------
    // Output limits
    // ------------------------------------------------------

    constexpr float OUTPUT_MIN_PERCENT =
        0.0f;

    constexpr float OUTPUT_MAX_PERCENT =
        100.0f;

    // ------------------------------------------------------
    // Integral protection
    // ------------------------------------------------------

    constexpr float INTEGRAL_MIN =
        -50.0f;

    constexpr float INTEGRAL_MAX =
        50.0f;

    // ------------------------------------------------------
    // Controller state
    // ------------------------------------------------------

    float targetRpm = 0.0f;

    float integralTerm = 0.0f;

    float previousErrorNormalized = 0.0f;

    float lastOutputPercent = 0.0f;

    SpeedControlState state{
        0.0f, // targetRpm
        0.0f, // measuredRpm
        0.0f, // errorRpm
        0.0f, // outputPercent
        0.0f, // integralTerm
        false // saturated
    };
}

// ==========================================================
// INITIALIZATION
// ==========================================================

void speedControllerInit(
    float newKp,
    float newKi,
    float newKd
)
{
    kp = newKp;
    ki = newKi;
    kd = newKd;

    speedControllerReset();
}

// ==========================================================
// SET TARGET
// ==========================================================

void speedControllerSetTarget(
    float newTargetRpm
)
{
    targetRpm =
        constrain(
            newTargetRpm,
            0.0f,
            MAX_SAFE_RPM
        );

    // Reset integral to prevent a sudden output jump
    // when target changes.

    integralTerm =
        0.0f;

    previousErrorNormalized =
        0.0f;
}

// ==========================================================
// UPDATE CONTROLLER
// ==========================================================

float speedControllerUpdate(
    float measuredRpm,
    float dtSeconds
)
{
    // ------------------------------------------------------
    // Invalid timestep
    // ------------------------------------------------------

    if (dtSeconds <= 0.0f)
    {
        return lastOutputPercent;
    }

    // ------------------------------------------------------
    // Sanitize measured RPM
    // ------------------------------------------------------

    measuredRpm =
        constrain(
            measuredRpm,
            0.0f,
            MAX_SAFE_RPM
        );

    // ------------------------------------------------------
    // Calculate error
    // ------------------------------------------------------

    const float errorRpm =
        targetRpm -
        measuredRpm;

    const float errorNormalized =
        errorRpm /
        MAX_SAFE_RPM;

    // ------------------------------------------------------
    // Integral
    // ------------------------------------------------------

    integralTerm +=
        errorNormalized *
        ki *
        dtSeconds;

    integralTerm =
        constrain(
            integralTerm,
            INTEGRAL_MIN,
            INTEGRAL_MAX
        );

    // ------------------------------------------------------
    // Derivative
    // ------------------------------------------------------

    const float derivativeNormalized =
        (
            errorNormalized -
            previousErrorNormalized
        ) /
        dtSeconds;

    // ------------------------------------------------------
    // PID output
    // ------------------------------------------------------

    const float proportional =
        kp *
        errorNormalized *
        100.0f;

    const float integral =
        integralTerm *
        100.0f;

    const float derivative =
        kd *
        derivativeNormalized *
        100.0f;

    const float rawOutput =
        proportional +
        integral +
        derivative;

    // ------------------------------------------------------
    // Clamp output
    // ------------------------------------------------------

    const float output =
        constrain(
            rawOutput,
            OUTPUT_MIN_PERCENT,
            OUTPUT_MAX_PERCENT
        );

    const bool saturated =
        output != rawOutput;

    // ------------------------------------------------------
    // Anti-windup
    // ------------------------------------------------------

    if (saturated)
    {
        const bool pushingUpper =
            (
                rawOutput >
                OUTPUT_MAX_PERCENT
            ) &&
            (
                errorNormalized >
                0.0f
            );

        const bool pushingLower =
            (
                rawOutput <
                OUTPUT_MIN_PERCENT
            ) &&
            (
                errorNormalized <
                0.0f
            );

        if (
            pushingUpper ||
            pushingLower
        )
        {
            integralTerm -=
                errorNormalized *
                ki *
                dtSeconds;
        }
    }

    // ------------------------------------------------------
    // Update state
    // ------------------------------------------------------

    state.targetRpm =
        targetRpm;

    state.measuredRpm =
        measuredRpm;

    state.errorRpm =
        errorRpm;

    state.outputPercent =
        output;

    state.integralTerm =
        integralTerm;

    state.saturated =
        saturated;

    previousErrorNormalized =
        errorNormalized;

    lastOutputPercent =
        output;

    return output;
}

// ==========================================================
// GET STATE
// ==========================================================

SpeedControlState speedControllerGetState()
{
    return state;
}

// ==========================================================
// RESET
// ==========================================================

void speedControllerReset()
{
    integralTerm =
        0.0f;

    previousErrorNormalized =
        0.0f;

    lastOutputPercent =
        0.0f;

    state.targetRpm =
        targetRpm;

    state.measuredRpm =
        0.0f;

    state.errorRpm =
        0.0f;

    state.outputPercent =
        0.0f;

    state.integralTerm =
        0.0f;

    state.saturated =
        false;
}