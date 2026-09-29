#include "voltage_sensor.h"

#include <Arduino.h>

#include "pins.h"

// ==========================================================
// FlowSense-X
// Voltage Sensor Implementation
//
// ESP32 ADC
// Voltage Sensor -> GPIO35
//
// Default assumption:
// 0-25V voltage sensor module
//
// Typical divider ratio:
// Vin = Vout × 5
//
// IMPORTANT:
// The DIVIDER_RATIO should be calibrated according to the
// actual voltage sensor module being used.
// ==========================================================

namespace
{
    // ======================================================
    // Configuration
    // ======================================================

    constexpr uint8_t SENSOR_PIN =
        PIN_VOLTAGE_SENSOR;

    // ESP32 ADC configuration
    constexpr uint16_t ADC_MAX_VALUE =
        4095;

    constexpr float ADC_REFERENCE_V =
        3.3f;

    // ------------------------------------------------------
    // Voltage divider
    //
    // Example:
    // 25V input -> approximately 5V output
    //
    // If the module outputs 1/5 of input:
    // input voltage = ADC voltage × 5
    // ------------------------------------------------------

    constexpr float DIVIDER_RATIO =
        5.0f;

    // ------------------------------------------------------
    // Valid voltage range
    // ------------------------------------------------------

    constexpr float MIN_VALID_VOLTAGE =
        0.0f;

    constexpr float MAX_VALID_VOLTAGE =
        25.0f;

    // ------------------------------------------------------
    // Filtering
    // ------------------------------------------------------

    constexpr uint8_t VOLTAGE_SAMPLES =
        8;

    // ======================================================
    // Runtime state
    // ======================================================

    uint16_t rawAdc =
        0;

    float adcVoltageV =
        0.0f;

    float inputVoltageV =
        0.0f;

    bool initialized =
        false;

    bool valid =
        false;

    // ======================================================
    // Read ADC voltage
    // ======================================================

    float readAdcVoltage()
    {
        uint32_t sum =
            0;

        for (
            uint8_t i = 0;
            i < VOLTAGE_SAMPLES;
            ++i
        )
        {
            sum += analogRead(
                SENSOR_PIN
            );
        }

        const float averageRaw =
            static_cast<float>(sum) /
            static_cast<float>(VOLTAGE_SAMPLES);

        return
            (
                averageRaw *
                ADC_REFERENCE_V
            ) /
            static_cast<float>(ADC_MAX_VALUE);
    }
}

// ==========================================================
// Initialization
// ==========================================================

bool voltageSensorInit()
{
    pinMode(
        SENSOR_PIN,
        INPUT
    );

    analogReadResolution(
        12
    );

    // GPIO35 is an ADC input.
    analogSetPinAttenuation(
        SENSOR_PIN,
        ADC_11db
    );

    rawAdc =
        0;

    adcVoltageV =
        0.0f;

    inputVoltageV =
        0.0f;

    valid =
        false;

    initialized =
        true;

    return initialized;
}

// ==========================================================
// Update
// ==========================================================

void voltageSensorUpdate()
{
    if (!initialized)
    {
        valid =
            false;

        return;
    }

    // ------------------------------------------------------
    // Read filtered ADC voltage
    // ------------------------------------------------------

    adcVoltageV =
        readAdcVoltage();

    // ------------------------------------------------------
    // Read raw ADC value separately
    // ------------------------------------------------------

    rawAdc =
        analogRead(
            SENSOR_PIN
        );

    // ------------------------------------------------------
    // Convert ADC voltage to actual input voltage
    // ------------------------------------------------------

    inputVoltageV =
        adcVoltageV *
        DIVIDER_RATIO;

    // ------------------------------------------------------
    // Validate measurement
    // ------------------------------------------------------

    valid =
        (
            inputVoltageV >=
            MIN_VALID_VOLTAGE
        ) &&
        (
            inputVoltageV <=
            MAX_VALID_VOLTAGE
        );
}

// ==========================================================
// Read Battery / Input Voltage
// ==========================================================

float voltageSensorReadVoltage()
{
    return inputVoltageV;
}

// ==========================================================
// Read ADC Voltage
// ==========================================================

float voltageSensorReadAdcVoltage()
{
    return adcVoltageV;
}

// ==========================================================
// Read Raw ADC
// ==========================================================

uint16_t voltageSensorReadRaw()
{
    return rawAdc;
}

// ==========================================================
// Validity
// ==========================================================

bool voltageSensorIsValid()
{
    return valid;
}

// ==========================================================
// Complete State
// ==========================================================

VoltageSensorState voltageSensorGetState()
{
    return {
        inputVoltageV,
        adcVoltageV,
        rawAdc,
        valid
    };
}