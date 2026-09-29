#include "esc.h"

#include <ESP32Servo.h>

#include "pins.h"
#include "constants.h"

// ==========================================================
// FlowSense-X
// ESC Control Implementation
// ==========================================================

namespace
{
    // ------------------------------------------------------
    // ESC driver
    // ------------------------------------------------------

    Servo escServo;

    // ------------------------------------------------------
    // Driver state
    // ------------------------------------------------------

    bool initialized = false;

    uint16_t currentPulseUs =
        ESC_MIN_US;
}

// ==========================================================
// ESC INITIALIZATION
// ==========================================================

bool escInit()
{
    initialized = false;

    currentPulseUs =
        ESC_MIN_US;

    // ------------------------------------------------------
    // Standard RC ESC signal
    // ------------------------------------------------------

    escServo.setPeriodHertz(
        50
    );

    // ------------------------------------------------------
    // Attach ESC signal
    // ------------------------------------------------------

    const int attachResult =
        escServo.attach(
            PIN_ESC_SIGNAL,
            ESC_MIN_US,
            ESC_MAX_US
        );

    // ------------------------------------------------------
    // Validate attachment
    // ------------------------------------------------------

    if (
        attachResult < 0 ||
        !escServo.attached()
    )
    {
        return false;
    }

    initialized = true;

    // ------------------------------------------------------
    // Safety-first startup
    // ------------------------------------------------------

    escServo.writeMicroseconds(
        ESC_MIN_US
    );

    currentPulseUs =
        ESC_MIN_US;

    return true;
}

// ==========================================================
// ESC ARM
// ==========================================================

void escArm()
{
    if (!initialized)
    {
        return;
    }

    // ------------------------------------------------------
    // Minimum throttle during complete arm period
    // ------------------------------------------------------

    escServo.writeMicroseconds(
        ESC_MIN_US
    );

    currentPulseUs =
        ESC_MIN_US;

    delay(
        ESC_ARM_TIME_MS
    );
}

// ==========================================================
// ESC STOP
// ==========================================================

void escStop()
{
    if (!initialized)
    {
        return;
    }

    // ------------------------------------------------------
    // Immediate minimum-throttle command
    // ------------------------------------------------------

    escServo.writeMicroseconds(
        ESC_MIN_US
    );

    currentPulseUs =
        ESC_MIN_US;
}

// ==========================================================
// WRITE ESC PULSE
// ==========================================================

void escWriteMicroseconds(
    uint16_t pulseUs
)
{
    if (!initialized)
    {
        return;
    }

    // ------------------------------------------------------
    // Hard clamp
    // ------------------------------------------------------

    pulseUs =
        constrain(
            pulseUs,
            ESC_MIN_US,
            ESC_MAX_US
        );

    // ------------------------------------------------------
    // Send command
    // ------------------------------------------------------

    escServo.writeMicroseconds(
        pulseUs
    );

    currentPulseUs =
        pulseUs;
}

// ==========================================================
// GET CURRENT ESC PULSE
// ==========================================================

uint16_t escGetCurrentPulse()
{
    return currentPulseUs;
}

// ==========================================================
// INITIALIZATION STATUS
// ==========================================================

bool escIsInitialized()
{
    return initialized;
}