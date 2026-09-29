#ifndef FLOWSENSE_AUTO_PEAK_H
#define FLOWSENSE_AUTO_PEAK_H

#include <Arduino.h>

enum class AutoPeakPhase : uint8_t {
    Idle = 0,
    RampUp,
    PeakApproach,
    PeakHold,
    RampDown,
    Stop
};

struct AutoPeakConfig {
    float peakRpm;
    float stepRpm;
    uint32_t holdMs;
    uint32_t rampUpMs;
    uint32_t rampDownMs;
    float smoothFactor;
};

struct AutoPeakStatus {
    bool active;
    AutoPeakPhase phase;
    float targetRpm;
    float peakRpm;
    uint32_t phaseElapsedMs;
    uint32_t holdRemainingMs;
    uint32_t runCount;
};

void autoPeakInit();
void autoPeakUpdate(float dtSeconds);
void autoPeakStart();
void autoPeakStop();
void autoPeakSetConfig(const AutoPeakConfig& config);
AutoPeakConfig autoPeakGetConfig();
AutoPeakStatus autoPeakGetStatus();
const char* autoPeakPhaseToString(AutoPeakPhase phase);

#endif
