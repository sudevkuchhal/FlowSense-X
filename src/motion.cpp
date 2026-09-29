#include "motion.h"

#include "constants.h"

#include <math.h>

namespace
{
    // ======================================================
    // Motion configuration
    // ======================================================

    constexpr float PROFILE_DURATION_S = 6.0f;

    // ======================================================
    // Internal motion state
    // ======================================================

    MotionState state{
        0.0f,   // currentRpm
        0.0f,   // targetRpm
        0.0f,   // commandRpm
        0.0f,   // normalizedPosition
        false,  // running
        true    // complete
    };

    // ======================================================
    // Clamp RPM
    // ======================================================

    float clampRpm(float rpm)
    {
        return constrain(
            rpm,
            MOTOR_MIN_RPM,
            MOTOR_MAX_RPM
        );
    }
}

// ==========================================================
// INITIALIZATION
// ==========================================================

void motionInit()
{
    state.currentRpm = 0.0f;
    state.targetRpm = 0.0f;
    state.commandRpm = 0.0f;

    state.normalizedPosition = 0.0f;

    state.running = false;
    state.complete = true;
}

// ==========================================================
// SET TARGET RPM
// ==========================================================

void motionSetTarget(
    float targetRpm
)
{
    state.targetRpm =
        clampRpm(targetRpm);

    state.normalizedPosition = 0.0f;
    state.commandRpm = 0.0f;

    state.running = false;
    state.complete = false;
}

// ==========================================================
// START MOTION
// ==========================================================

void motionStart()
{
    if (state.targetRpm <= MOTOR_MIN_RPM)
    {
        state.running = false;
        state.complete = true;
        state.commandRpm = 0.0f;

        return;
    }

    state.normalizedPosition = 0.0f;
    state.commandRpm = 0.0f;

    state.running = true;
    state.complete = false;
}

// ==========================================================
// STOP MOTION
// ==========================================================

void motionStop()
{
    state.running = false;

    state.commandRpm = 0.0f;

    state.normalizedPosition = 0.0f;

    state.complete = true;
}

// ==========================================================
// UPDATE MOTION PROFILE
// ==========================================================

void motionUpdate(
    float dtSeconds
)
{
    if (
        !state.running ||
        state.complete ||
        dtSeconds <= 0.0f
    )
    {
        return;
    }

    // ------------------------------------------------------
    // Advance normalized motion position
    // ------------------------------------------------------

    state.normalizedPosition +=
        dtSeconds /
        PROFILE_DURATION_S;

    // ------------------------------------------------------
    // Profile complete
    // ------------------------------------------------------

    if (
        state.normalizedPosition >= 1.0f
    )
    {
        state.normalizedPosition = 1.0f;

        state.commandRpm =
            state.targetRpm;

        state.running = false;
        state.complete = true;

        return;
    }

    // ------------------------------------------------------
    // Normalized time
    // ------------------------------------------------------

    const float t =
        state.normalizedPosition;

    // ------------------------------------------------------
    // Quintic smootherstep
    //
    // s(t) = 6t^5 - 15t^4 + 10t^3
    //
    // Provides smooth acceleration/deceleration.
    // ------------------------------------------------------

    const float t2 = t * t;
    const float t3 = t2 * t;
    const float t4 = t3 * t;
    const float t5 = t4 * t;

    const float smooth =
        (6.0f * t5) -
        (15.0f * t4) +
        (10.0f * t3);

    // ------------------------------------------------------
    // Generate ESC command RPM
    // ------------------------------------------------------

    state.commandRpm =
        state.targetRpm *
        smooth;

    state.commandRpm =
        max(
            0.0f,
            state.commandRpm
        );
}

// ==========================================================
// UPDATE MEASURED RPM
// ==========================================================
//
// Actual RPM comes from the IR sensor.
//
// This keeps motion control separate from measurement.
//
// ==========================================================

void motionSetMeasuredRpm(
    float measuredRpm
)
{
    state.currentRpm =
        max(
            0.0f,
            measuredRpm
        );
}

// ==========================================================
// GET STATE
// ==========================================================

MotionState motionGetState()
{
    return state;
}