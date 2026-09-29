#ifndef FLOWSENSE_EVENTS_H
#define FLOWSENSE_EVENTS_H

#include <Arduino.h>

// ==========================================================
// FlowSense-X
// Event / Fault Manager Interface
// ==========================================================
//
// Handles:
//   - Over-current
//   - Over-speed
//   - Sensor faults
//   - Invalid target
//   - Emergency stop
//   - Warning
//   - Recovery
//
// ==========================================================

// ==========================================================
// FAULT CODES
// ==========================================================

enum class FaultCode : uint8_t
{
    None = 0,

    OverCurrent,

    OverSpeed,

    SensorFault,

    InvalidCommand,

    EmergencyStop
};

// ==========================================================
// EVENT TYPES
// ==========================================================

enum class EventType : uint8_t
{
    None = 0,

    Warning,

    Fault,

    Recovery,

    Emergency
};

// ==========================================================
// EVENT STATUS
// ==========================================================

struct EventStatus
{
    FaultCode fault;

    EventType type;

    bool active;

    uint32_t timestampMs;

    uint32_t sequence;
};

// ==========================================================
// INITIALIZATION
// ==========================================================

void eventsInit();

// ==========================================================
// CLEAR / RECOVERY
// ==========================================================

void eventsClear();

// ==========================================================
// PROCESS SYSTEM CONDITIONS
// ==========================================================
//
// currentA:
//     Current measured by ACS712.
//
// measuredRpm:
//     RPM measured from IR reflective sensor.
//
// targetRpm:
//     Requested motor target.
//
// currentValid:
//     ACS712 data validity.
//
// rpmValid:
//     IR RPM feedback validity.
//
// emergencyRequest:
//     Software emergency-stop request.
//
// ==========================================================

void eventsProcess(
    float currentA,
    float measuredRpm,
    float targetRpm,
    bool currentValid,
    bool rpmValid,
    bool emergencyRequest
);

// ==========================================================
// STATUS ACCESS
// ==========================================================

EventStatus eventsGetStatus();

FaultCode eventsGetFault();

EventType eventsGetType();

bool eventsHasActiveFault();

// ==========================================================
// STRING CONVERSION
// ==========================================================

const char* faultCodeToString(
    FaultCode fault
);

const char* eventTypeToString(
    EventType type
);

#endif // FLOWSENSE_EVENTS_H