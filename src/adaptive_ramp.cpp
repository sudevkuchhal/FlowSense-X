#include "adaptive_ramp.h"

namespace
{
constexpr float MIN_ACCELERATION_RPM_PER_SEC = 50.0f;
constexpr float MAX_ACCELERATION_RPM_PER_SEC = 1200.0f;

constexpr float HIGH_CURRENT_A = 10.0f;
constexpr float NEAR_TARGET_RPM = 300.0f;

float baseAcceleration = 800.0f;
float effectiveAcceleration = 800.0f;
float targetRpm = 0.0f;

AdaptiveRampState state{
    800.0f,
    800.0f,
    0.0f,
    0.0f,
    0.0f,
    false,
    false
};
}

void adaptiveRampInit(float baseAccelerationRpmPerSec)
{
    baseAcceleration = constrain(
        baseAccelerationRpmPerSec,
        MIN_ACCELERATION_RPM_PER_SEC,
        MAX_ACCELERATION_RPM_PER_SEC
    );

    effectiveAcceleration = baseAcceleration;

    state.baseAccelerationRpmPerSec = baseAcceleration;
    state.effectiveAccelerationRpmPerSec = effectiveAcceleration;
    state.currentRpm = 0.0f;
    state.targetRpm = targetRpm;
    state.measuredCurrentA = 0.0f;
    state.currentLimited = false;
    state.targetLimited = false;
}

void adaptiveRampSetTarget(float newTargetRpm)
{
    targetRpm = max(0.0f, newTargetRpm);
    state.targetRpm = targetRpm;
}

void adaptiveRampSetBaseAcceleration(float accelerationRpmPerSec)
{
    baseAcceleration = constrain(
        accelerationRpmPerSec,
        MIN_ACCELERATION_RPM_PER_SEC,
        MAX_ACCELERATION_RPM_PER_SEC
    );
    effectiveAcceleration = baseAcceleration;
    state.baseAccelerationRpmPerSec = baseAcceleration;
    state.effectiveAccelerationRpmPerSec = effectiveAcceleration;
}

void adaptiveRampUpdate(float currentRpm, float measuredCurrentA)
{
    currentRpm = max(0.0f, currentRpm);
    measuredCurrentA = max(0.0f, measuredCurrentA);

    effectiveAcceleration = baseAcceleration;

    bool currentLimited = false;
    bool targetLimited = false;

    if (measuredCurrentA >= HIGH_CURRENT_A)
    {
        effectiveAcceleration *= 0.5f;
        currentLimited = true;
    }

    const float errorRpm = fabs(targetRpm - currentRpm);

    if (errorRpm <= NEAR_TARGET_RPM)
    {
        effectiveAcceleration *= 0.5f;
        targetLimited = true;
    }

    effectiveAcceleration = constrain(
        effectiveAcceleration,
        MIN_ACCELERATION_RPM_PER_SEC,
        MAX_ACCELERATION_RPM_PER_SEC
    );

    state.baseAccelerationRpmPerSec = baseAcceleration;
    state.effectiveAccelerationRpmPerSec = effectiveAcceleration;
    state.currentRpm = currentRpm;
    state.targetRpm = targetRpm;
    state.measuredCurrentA = measuredCurrentA;
    state.currentLimited = currentLimited;
    state.targetLimited = targetLimited;
}

float adaptiveRampGetCommandRpm(
    float currentRpm,
    float dtSeconds
)
{
    if (dtSeconds <= 0.0f)
        return currentRpm;

    const float difference = targetRpm - currentRpm;
    const float maxStep = effectiveAcceleration * dtSeconds;

    if (difference > maxStep)
        return currentRpm + maxStep;

    if (difference < -maxStep)
        return currentRpm - maxStep;

    return targetRpm;
}

AdaptiveRampState adaptiveRampGetState()
{
    return state;
}