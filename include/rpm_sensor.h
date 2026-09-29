#ifndef FLOWSENSE_RPM_SENSOR_H
#define FLOWSENSE_RPM_SENSOR_H

#include <Arduino.h>

// ==========================================================
// FlowSense-X
// IR Reflective RPM Sensor
// ==========================================================
//
// Hardware:
//   IR reflective sensor -> GPIO33
//   Motor shaft          -> one black reflective mark
//
// Every detected black-mark transition = 1 pulse.
//
// The sensor ONLY measures rotation.
// It NEVER stops the motor.
//
// ==========================================================

struct RpmSensorState
{
    float rpm;

    uint32_t pulseCount;

    uint32_t pulsesPerSecond;

    bool valid;
    bool signalPresent;
};

// ==========================================================
// Initialization
// ==========================================================

bool rpmSensorInit();

// ==========================================================
// Periodic update
// ==========================================================

void rpmSensorUpdate();

// ==========================================================
// Measurements
// ==========================================================

float rpmSensorGetRpm();

uint32_t rpmSensorGetPulseCount();

uint32_t rpmSensorGetPulsesPerSecond();

// ==========================================================
// Status
// ==========================================================

bool rpmSensorIsValid();

bool rpmSensorSignalPresent();

// ==========================================================
// Complete state
// ==========================================================

RpmSensorState rpmSensorGetState();

#endif // FLOWSENSE_RPM_SENSOR_H