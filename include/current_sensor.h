#ifndef FLOWSENSE_CURRENT_SENSOR_H
#define FLOWSENSE_CURRENT_SENSOR_H

#include <Arduino.h>

struct CurrentSensorState
{
    float currentA;
    float sensorVoltageV;
    float zeroOffsetV;
    bool calibrated;
    bool valid;
};

bool currentSensorInit();
void currentSensorCalibrateZero();

void currentSensorUpdate();

float currentSensorReadVoltage();
float currentSensorReadCurrent();

float currentSensorGetZeroOffsetV();

bool currentSensorIsCalibrated();
bool currentSensorIsValid();

CurrentSensorState currentSensorGetState();

#endif