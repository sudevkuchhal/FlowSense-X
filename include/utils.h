#ifndef FLOWSENSE_UTILS_H
#define FLOWSENSE_UTILS_H

#include <Arduino.h>

// ==========================================================
// FlowSense-X
// Utility Functions
// ==========================================================

float clampFloat(
    float value,
    float minimum,
    float maximum
);

uint16_t clampPulse(
    uint16_t pulseUs,
    uint16_t minimum,
    uint16_t maximum
);

float mapFloat(
    float value,
    float inMin,
    float inMax,
    float outMin,
    float outMax
);

float rpmToPercent(
    float rpm,
    float maxRpm
);

uint16_t rpmToEscPulse(
    float rpm,
    float minRpm,
    float maxRpm,
    uint16_t minPulseUs,
    uint16_t maxPulseUs
);

const char* boolToString(
    bool value
);

#endif // FLOWSENSE_UTILS_H