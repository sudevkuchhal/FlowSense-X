#ifndef FLOWSENSE_VOLTAGE_SENSOR_H
#define FLOWSENSE_VOLTAGE_SENSOR_H

#include <Arduino.h>

// ==========================================================
// FlowSense-X
// Voltage Sensor Interface
//
// Hardware:
// ESP32 DevKit V1
// Voltage Sensor -> GPIO35
//
// Provides:
// - Raw ADC value
// - ADC voltage
// - Calculated battery/input voltage
// - Sensor validity
// ==========================================================

struct VoltageSensorState
{
    float voltageV;
    float adcVoltageV;

    uint16_t rawAdc;

    bool valid;
};

// ==========================================================
// Initialization
// ==========================================================

bool voltageSensorInit();

// ==========================================================
// Runtime Update
// ==========================================================

void voltageSensorUpdate();

// ==========================================================
// Read Functions
// ==========================================================

float voltageSensorReadVoltage();

float voltageSensorReadAdcVoltage();

uint16_t voltageSensorReadRaw();

// ==========================================================
// Status
// ==========================================================

bool voltageSensorIsValid();

VoltageSensorState voltageSensorGetState();

#endif // FLOWSENSE_VOLTAGE_SENSOR_H