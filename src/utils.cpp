#include "utils.h"

// ==========================================================
// FlowSense-X
// Utility Functions Implementation
// ==========================================================

// ==========================================================
// Float Clamp
// ==========================================================

float clampFloat(
    float value,
    float minimum,
    float maximum
)
{
    if (minimum > maximum)
    {
        const float temp = minimum;
        minimum = maximum;
        maximum = temp;
    }

    if (value < minimum)
    {
        return minimum;
    }

    if (value > maximum)
    {
        return maximum;
    }

    return value;
}

// ==========================================================
// ESC Pulse Clamp
// ==========================================================

uint16_t clampPulse(
    uint16_t pulseUs,
    uint16_t minimum,
    uint16_t maximum
)
{
    if (minimum > maximum)
    {
        const uint16_t temp = minimum;
        minimum = maximum;
        maximum = temp;
    }

    if (pulseUs < minimum)
    {
        return minimum;
    }

    if (pulseUs > maximum)
    {
        return maximum;
    }

    return pulseUs;
}

// ==========================================================
// Floating Point Map
// ==========================================================

float mapFloat(
    float value,
    float inMin,
    float inMax,
    float outMin,
    float outMax
)
{
    if (inMax == inMin)
    {
        return outMin;
    }

    const float ratio =
        (value - inMin) /
        (inMax - inMin);

    return
        outMin +
        ratio *
        (outMax - outMin);
}

// ==========================================================
// RPM → Percentage
// ==========================================================

float rpmToPercent(
    float rpm,
    float maxRpm
)
{
    if (maxRpm <= 0.0f)
    {
        return 0.0f;
    }

    rpm =
        clampFloat(
            rpm,
            0.0f,
            maxRpm
        );

    return
        (rpm / maxRpm) *
        100.0f;
}

// ==========================================================
// RPM → ESC Pulse
// ==========================================================

uint16_t rpmToEscPulse(
    float rpm,
    float minRpm,
    float maxRpm,
    uint16_t minPulseUs,
    uint16_t maxPulseUs
)
{
    if (maxRpm <= minRpm)
    {
        return minPulseUs;
    }

    rpm =
        clampFloat(
            rpm,
            minRpm,
            maxRpm
        );

    const float pulse =
        mapFloat(
            rpm,
            minRpm,
            maxRpm,
            static_cast<float>(minPulseUs),
            static_cast<float>(maxPulseUs)
        );

    return static_cast<uint16_t>(
        pulse
    );
}

// ==========================================================
// Boolean → Text
// ==========================================================

const char* boolToString(
    bool value
)
{
    return value
        ? "true"
        : "false";
}