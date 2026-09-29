#ifndef FLOWSENSE_ADAPTIVE_RAMP_H
#define FLOWSENSE_ADAPTIVE_RAMP_H

#include <Arduino.h>

struct AdaptiveRampState
{
    float baseAccelerationRpmPerSec;
    float effectiveAccelerationRpmPerSec;
    float currentRpm;
    float targetRpm;
    float measuredCurrentA;
    bool currentLimited;
    bool targetLimited;
};

void adaptiveRampInit(float baseAccelerationRpmPerSec);
void adaptiveRampSetTarget(float targetRpm);
void adaptiveRampSetBaseAcceleration(float accelerationRpmPerSec);
void adaptiveRampUpdate(float currentRpm, float measuredCurrentA);

float adaptiveRampGetCommandRpm(
    float currentRpm,
    float dtSeconds
);

AdaptiveRampState adaptiveRampGetState();

#endif