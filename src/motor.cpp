#include "motor.h"

#include "esc.h"
#include "constants.h"

// ==========================================================
// FlowSense-X
// Motor Control Implementation
// ==========================================================

namespace
{
    bool initialized = false;
    bool enabled = false;

    uint16_t currentPulseUs = ESC_MIN_US;
}

// ==========================================================
// Initialization
// ==========================================================

bool motorInit()
{
    initialized = false;
    enabled = false;
    currentPulseUs = ESC_MIN_US;

    if (!escInit())
    {
        return false;
    }

    escStop();

    initialized = true;

    return true;
}

// ==========================================================
// Start
// ==========================================================

void motorStart()
{
    if (!initialized)
    {
        return;
    }

    escArm();

    enabled = true;

    currentPulseUs =
        ESC_MIN_US;

    escWriteMicroseconds(
        currentPulseUs
    );
}

// ==========================================================
// Stop
// ==========================================================

void motorStop()
{
    if (!initialized)
    {
        return;
    }

    escStop();

    currentPulseUs =
        ESC_MIN_US;

    enabled = false;
}

// ==========================================================
// Set ESC Pulse
// ==========================================================

void motorSetPulse(
    uint16_t pulseUs
)
{
    if (!initialized)
    {
        return;
    }

    if (!enabled)
    {
        pulseUs =
            ESC_MIN_US;
    }

    pulseUs =
        constrain(
            pulseUs,
            ESC_MIN_US,
            ESC_MAX_US
        );

    escWriteMicroseconds(
        pulseUs
    );

    currentPulseUs =
        escGetCurrentPulse();
}

// ==========================================================
// Get Pulse
// ==========================================================

uint16_t motorGetPulse()
{
    return currentPulseUs;
}

// ==========================================================
// Status
// ==========================================================

bool motorIsInitialized()
{
    return initialized;
}

bool motorIsEnabled()
{
    return enabled;
}

// ==========================================================
// State
// ==========================================================

MotorState motorGetState()
{
    return {
        initialized,
        enabled,
        currentPulseUs
    };
}