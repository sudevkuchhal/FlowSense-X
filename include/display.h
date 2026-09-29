#ifndef FLOWSENSE_DISPLAY_H
#define FLOWSENSE_DISPLAY_H

#include <Arduino.h>

// ==========================================================
// FlowSense-X
// Unified OLED Dashboard
//
// OLED: SSD1306 128x64
//
// Display only.
// No motor-control logic is performed here.
// ==========================================================

enum class DisplayPage : uint8_t
{
    System = 0,
    Motor,
    Current,
    Voltage,
    ESC,
    Safety
};

// ==========================================================
// Display API
// ==========================================================

bool displayInit();

void displayClear();

void displayShowBoot();

void displayShowPage(
    DisplayPage page,

    float targetRpm,
    float commandRpm,
    float measuredRpm,

    float currentA,
    float voltageV,

    uint16_t escPulseUs,

    bool safetyAllowed,

    const char* systemState
);

#endif // FLOWSENSE_DISPLAY_H