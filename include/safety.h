#ifndef FLOWSENSE_SAFETY_H
#define FLOWSENSE_SAFETY_H

#include <Arduino.h>

// ==========================================================
// FlowSense-X
// M12 Safety Manager
// ==========================================================
//
// Responsibilities:
//   - Emergency-stop handling
//   - RPM overspeed protection
//   - Current protection
//   - Sensor validity checking
//   - Target RPM validation
//
// IMPORTANT:
// IR RPM detection itself NEVER stops the motor.
// Safety only reacts when an actual safety limit is exceeded.
//
// ==========================================================

enum class SafetyState : uint8_t
{
    Safe = 0,
    Warning,
    Fault,
    EmergencyStop
};

// ==========================================================
// Safety Fault Codes
// ==========================================================

enum class SafetyFault : uint8_t
{
    None = 0,
    OverCurrent,
    OverSpeed,
    SensorFault,
    InvalidCommand,
    EmergencyRequest
};

// ==========================================================
// Safety Input
// ==========================================================

struct SafetyInput
{
    float currentA;
    float rpm;

    bool currentSensorValid;
    bool rpmSensorValid;

    float targetRpm;

    bool emergencyRequest;
};

// ==========================================================
// Safety Status
// ==========================================================

struct SafetyStatus
{
    SafetyState state;
    SafetyFault fault;

    bool throttleAllowed;
    bool motorAllowed;
};

// ==========================================================
// API
// ==========================================================

void safetyInit();

void safetyUpdate(
    const SafetyInput& input
);

SafetyStatus safetyGetStatus();

void safetyClearFault();

bool safetyIsMotorAllowed();

bool safetyIsThrottleAllowed();

#endif // FLOWSENSE_SAFETY_H