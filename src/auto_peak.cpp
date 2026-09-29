#include "auto_peak.h"
#include "constants.h"
#include "system.h"
#include "adaptive_ramp.h"

namespace {
AutoPeakConfig cfg{14000.0f, 500.0f, 3000UL, 12000UL, 8000UL, 1.0f};
AutoPeakStatus st{false, AutoPeakPhase::Idle, 0.0f, 14000.0f, 0, 0, 0};
uint32_t phaseStartMs = 0;
float commandedTarget = 0.0f;

float clampPeak(float v) { return constrain(v, 1000.0f, MAX_SAFE_RPM); }
float clampStep(float v) { return constrain(v, 100.0f, 2000.0f); }
uint32_t clampTime(uint32_t v) { return constrain(v, 500UL, 120000UL); }

void setPhase(AutoPeakPhase p) {
    st.phase = p;
    phaseStartMs = millis();
}
}

void autoPeakInit() {
    phaseStartMs = millis();
    st = {false, AutoPeakPhase::Idle, 0.0f, cfg.peakRpm, 0, 0, 0};
    commandedTarget = 0.0f;
}

void autoPeakSetConfig(const AutoPeakConfig& in) {
    cfg.peakRpm = clampPeak(in.peakRpm);
    cfg.stepRpm = clampStep(in.stepRpm);
    cfg.holdMs = clampTime(in.holdMs);
    cfg.rampUpMs = clampTime(in.rampUpMs);
    cfg.rampDownMs = clampTime(in.rampDownMs);
    cfg.smoothFactor = constrain(in.smoothFactor, 0.25f, 1.5f);
    st.peakRpm = cfg.peakRpm;
    adaptiveRampSetBaseAcceleration(800.0f * cfg.smoothFactor);
}

AutoPeakConfig autoPeakGetConfig() { return cfg; }
AutoPeakStatus autoPeakGetStatus() { st.phaseElapsedMs = millis() - phaseStartMs; return st; }

void autoPeakStart() {
    if (st.active) return;
    st.active = true;
    st.runCount++;
    st.peakRpm = cfg.peakRpm;
    adaptiveRampSetBaseAcceleration(800.0f * cfg.smoothFactor);
    st.targetRpm = 0.0f;
    st.holdRemainingMs = cfg.holdMs;
    commandedTarget = 0.0f;
    setPhase(AutoPeakPhase::RampUp);
    systemSetTargetRpm(0.0f);
}

void autoPeakStop() {
    st.active = false;
    commandedTarget = 0.0f;
    st.targetRpm = 0.0f;
    st.holdRemainingMs = 0;
    setPhase(AutoPeakPhase::Idle);
    systemSetTargetRpm(0.0f);
}

void autoPeakUpdate(float dtSeconds) {
    if (!st.active || dtSeconds <= 0.0f) return;
    const uint32_t elapsed = millis() - phaseStartMs;

    if (st.phase == AutoPeakPhase::RampUp || st.phase == AutoPeakPhase::PeakApproach) {
        const float rate = (cfg.peakRpm / (cfg.rampUpMs / 1000.0f)) * cfg.smoothFactor;
        commandedTarget += rate * dtSeconds;
        commandedTarget = min(commandedTarget, cfg.peakRpm);
        if (commandedTarget >= cfg.peakRpm) {
            commandedTarget = cfg.peakRpm;
            st.targetRpm = commandedTarget;
            setPhase(AutoPeakPhase::PeakHold);
            st.holdRemainingMs = cfg.holdMs;
        } else {
            st.targetRpm = commandedTarget;
            if (st.phase == AutoPeakPhase::RampUp) setPhase(AutoPeakPhase::PeakApproach);
        }
    } else if (st.phase == AutoPeakPhase::PeakHold) {
        systemSetTargetRpm(cfg.peakRpm);
        st.targetRpm = cfg.peakRpm;
        st.holdRemainingMs = elapsed >= cfg.holdMs ? 0 : cfg.holdMs - elapsed;
        if (elapsed >= cfg.holdMs) {
            commandedTarget = cfg.peakRpm;
            setPhase(AutoPeakPhase::RampDown);
        }
    } else if (st.phase == AutoPeakPhase::RampDown) {
        const float rate = (cfg.peakRpm / (cfg.rampDownMs / 1000.0f)) * cfg.smoothFactor;
        commandedTarget -= rate * dtSeconds;
        commandedTarget = max(commandedTarget, 0.0f);
        st.targetRpm = commandedTarget;
        if (commandedTarget <= 0.0f) {
            commandedTarget = 0.0f;
            systemSetTargetRpm(0.0f);
            setPhase(AutoPeakPhase::Stop);
        } else {
            systemSetTargetRpm(commandedTarget);
        }
    } else if (st.phase == AutoPeakPhase::Stop) {
        systemSetTargetRpm(0.0f);
        st.targetRpm = 0.0f;
        st.active = false;
        setPhase(AutoPeakPhase::Idle);
    }

    if (st.phase != AutoPeakPhase::PeakHold && st.phase != AutoPeakPhase::Idle)
        systemSetTargetRpm(st.targetRpm);
}

const char* autoPeakPhaseToString(AutoPeakPhase phase) {
    switch (phase) {
        case AutoPeakPhase::RampUp: return "RAMP UP";
        case AutoPeakPhase::PeakApproach: return "PEAK APPROACH";
        case AutoPeakPhase::PeakHold: return "PEAK HOLD";
        case AutoPeakPhase::RampDown: return "RAMP DOWN";
        case AutoPeakPhase::Stop: return "STOP";
        default: return "IDLE";
    }
}
