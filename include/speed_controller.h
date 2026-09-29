#ifndef FLOWSENSE_SPEED_CONTROLLER_H
#define FLOWSENSE_SPEED_CONTROLLER_H

#include <Arduino.h>

// ==========================================================
// FlowSense-X
// M10 Closed-Loop Speed Controller
// ==========================================================
//
// Input:
//     Actual RPM from IR RPM sensor
//
// Target:
//     Requested RPM
//
// Output:
//     Throttle percentage: 0–100%
//
// The motor/ESC layer converts this percentage into
// the actual ESC pulse width.
//
// ==========================================================

struct SpeedControlState
{
    float targetRpm;
    float measuredRpm;
    float errorRpm;

    float outputPercent;

    float integralTerm;

    bool saturated;
};

// ==========================================================
// Initialization
// ==========================================================

void speedControllerInit(
    float kp,
    float ki,
    float kd = 0.0f
);

// ==========================================================
// Target RPM
// ==========================================================

void speedControllerSetTarget(
    float targetRpm
);

// ==========================================================
// Controller Update
// ==========================================================

float speedControllerUpdate(
    float measuredRpm,
    float dtSeconds
);

// ==========================================================
// State
// ==========================================================

SpeedControlState speedControllerGetState();

// ==========================================================
// Reset
// ==========================================================

void speedControllerReset();

#endif // FLOWSENSE_SPEED_CONTROLLER_H