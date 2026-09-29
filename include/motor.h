#ifndef FLOWSENSE_MOTOR_H
#define FLOWSENSE_MOTOR_H

#include <Arduino.h>

// ==========================================================
// FlowSense-X
// Motor Control Interface
// ==========================================================

struct MotorState
{
    bool initialized;
    bool enabled;

    uint16_t pulseUs;
};

bool motorInit();

void motorStart();

void motorStop();

void motorSetPulse(uint16_t pulseUs);

uint16_t motorGetPulse();

bool motorIsInitialized();

bool motorIsEnabled();

MotorState motorGetState();

#endif // FLOWSENSE_MOTOR_H
