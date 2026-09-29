#ifndef FLOWSENSE_SYSTEM_H
#define FLOWSENSE_SYSTEM_H

#include <Arduino.h>

#include "events.h"

// ==========================================================
// FlowSense-X
// System Core Interface
//
// Responsibilities:
// - Target RPM management
// - Motion/ramp coordination
// - RPM feedback
// - Current/voltage telemetry
// - Safety integration
// - Event integration
// - ESC command generation
// ==========================================================

enum class SystemState : uint8_t
{
    Idle = 0,
    Running,
    Warning,
    Fault,
    EmergencyStop
};

// ==========================================================
// System Telemetry
// ==========================================================

struct SystemStatus
{
    // ------------------------------------------------------
    // System
    // ------------------------------------------------------

    SystemState state;

    // ------------------------------------------------------
    // Speed
    // ------------------------------------------------------

    float targetRpm;
    float commandRpm;
    float measuredRpm;

    // ------------------------------------------------------
    // Electrical
    // ------------------------------------------------------

    float currentA;
    float voltageV;

    // ------------------------------------------------------
    // ESC
    // ------------------------------------------------------

    uint16_t requestedEscPulseUs;
    uint16_t escPulseUs;

    // ------------------------------------------------------
    // Safety
    // ------------------------------------------------------

    bool safetyAllowed;

    // ------------------------------------------------------
    // Event / Fault
    // ------------------------------------------------------

    FaultCode fault;
    EventType eventType;
};

// ==========================================================
// Initialization
// ==========================================================

void systemInit();

// ==========================================================
// Target Control
// ==========================================================

void systemSetTargetRpm(
    float targetRpm
);

// ==========================================================
// RPM Feedback
// ==========================================================
//
// Called by the RPM sensor module.
//
// IR sensor:
// black-mark detection -> pulse count -> RPM
// ==========================================================

void systemSetMeasuredRpm(
    float rpm
);

// ==========================================================
// Electrical Telemetry
// ==========================================================

void systemSetCurrent(
    float currentA
);

void systemSetVoltage(
    float voltageV
);

// ==========================================================
// ESC Synchronization
// ==========================================================

void systemSetActualEscPulse(
    uint16_t pulseUs
);

// ==========================================================
// Emergency Stop
// ==========================================================

void systemRequestEmergencyStop(
    bool request
);

// ==========================================================
// Main Update
// ==========================================================

void systemUpdate(
    float dtSeconds
);

// ==========================================================
// Status
// ==========================================================

SystemStatus systemGetStatus();

#endif // FLOWSENSE_SYSTEM_H