#ifndef FLOWSENSE_ESC_H
#define FLOWSENSE_ESC_H

#include <Arduino.h>

// ==========================================================
// FlowSense-X
// ESC Control Interface
//
// Responsibilities:
// - Initialize ESC PWM
// - Arm ESC at minimum throttle
// - Write validated ESC pulse width
// - Stop motor
// - Report current ESC command
//
// Motor control policy remains in system/motor layer.
// ==========================================================

bool escInit();

void escArm();

void escStop();

void escWriteMicroseconds(
    uint16_t pulseUs
);

uint16_t escGetCurrentPulse();

bool escIsInitialized();

#endif // FLOWSENSE_ESC_H