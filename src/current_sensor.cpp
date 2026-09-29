#include "current_sensor.h"

#include <Arduino.h>
#include <math.h>

#include "pins.h"
#include "constants.h"

namespace
{
float zeroOffsetV = 0.0f;
float sensorVoltageV = 0.0f;
float currentA = 0.0f;

bool calibrated = false;
bool valid = false;

float readAdcVoltage()
{
    return static_cast<float>(
        analogReadMilliVolts(PIN_CURRENT_SENSOR)
    ) / 1000.0f;
}

float readSensorVoltage()
{
    const float adcVoltageV = readAdcVoltage();

    if (!ACS712_USE_DIVIDER)
        return adcVoltageV;

    if (ACS712_DIVIDER_RATIO <= 0.0f)
        return 0.0f;

    return adcVoltageV / ACS712_DIVIDER_RATIO;
}
}

bool currentSensorInit()
{
    pinMode(PIN_CURRENT_SENSOR, INPUT);

    analogReadResolution(12);

    analogSetPinAttenuation(
        PIN_CURRENT_SENSOR,
        ADC_11db
    );

    delay(200);

    currentSensorCalibrateZero();

    // The zero point is calibrated at startup, so the absolute
    // ACS712 zero voltage must not be used as a hard fault
    // criterion. With the external divider, different ACS712
    // modules/clones can present different zero levels.
    //
    // Validate that GPIO34 is actually seeing a sane ADC voltage
    // and then use the measured zeroOffsetV for current calculation.
    const float zeroAdcVoltageV =
        zeroOffsetV * ACS712_DIVIDER_RATIO;

    calibrated =
        zeroAdcVoltageV >= 0.20f &&
        zeroAdcVoltageV <= 3.10f;

    Serial.print("[CURRENT] Zero offset: ");
    Serial.print(zeroOffsetV, 3);
    Serial.println(" V");

    valid = calibrated;

    return calibrated;
}

void currentSensorCalibrateZero()
{
    uint64_t sumMv = 0;

    for (
        uint16_t i = 0;
        i < ACS712_ZERO_CAL_SAMPLES;
        ++i
    )
    {
        sumMv += analogReadMilliVolts(
            PIN_CURRENT_SENSOR
        );

        delayMicroseconds(200);
    }

    const float averageAdcVoltageV =
        static_cast<float>(sumMv) /
        static_cast<float>(ACS712_ZERO_CAL_SAMPLES) /
        1000.0f;

    if (
        ACS712_USE_DIVIDER &&
        ACS712_DIVIDER_RATIO > 0.0f
    )
    {
        zeroOffsetV =
            averageAdcVoltageV /
            ACS712_DIVIDER_RATIO;
    }
    else
    {
        zeroOffsetV = averageAdcVoltageV;
    }
}

float currentSensorReadVoltage()
{
    float sumVoltage = 0.0f;

    for (
        uint16_t i = 0;
        i < ACS712_READ_SAMPLES;
        ++i
    )
    {
        sumVoltage += readSensorVoltage();
    }

    return sumVoltage /
           static_cast<float>(ACS712_READ_SAMPLES);
}

float currentSensorReadCurrent()
{
    if (!calibrated)
        return 0.0f;

    sensorVoltageV =
        currentSensorReadVoltage();

    float measuredCurrentA =
        (sensorVoltageV - zeroOffsetV) /
        ACS712_SENSITIVITY;

    if (
        fabsf(measuredCurrentA) <
        ACS712_NOISE_DEADBAND_A
    )
    {
        measuredCurrentA = 0.0f;
    }

    return fabsf(measuredCurrentA);
}

void currentSensorUpdate()
{
    if (!calibrated)
    {
        currentA = 0.0f;
        valid = false;
        return;
    }

    currentA = currentSensorReadCurrent();
    valid = true;
}

float currentSensorGetZeroOffsetV()
{
    return zeroOffsetV;
}

bool currentSensorIsCalibrated()
{
    return calibrated;
}

bool currentSensorIsValid()
{
    return valid;
}

CurrentSensorState currentSensorGetState()
{
    return {
        currentA,
        sensorVoltageV,
        zeroOffsetV,
        calibrated,
        valid
    };
}