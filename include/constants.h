#ifndef FLOWSENSE_CONSTANTS_H
#define FLOWSENSE_CONSTANTS_H

#include <Arduino.h>

// ==========================================================
// FlowSense-X
// Final Hardware Configuration
// ==========================================================


// ==========================================================
// SERIAL
// ==========================================================

constexpr uint32_t SERIAL_BAUD_RATE = 115200;
constexpr uint32_t STARTUP_DELAY_MS = 1000;


// ==========================================================
// ESC / BLDC
// ==========================================================

constexpr uint16_t ESC_MIN_US = 1000;
constexpr uint16_t ESC_MAX_US = 2000;

constexpr uint16_t ESC_ARM_TIME_MS = 3000;

// Safe startup throttle
constexpr float DEFAULT_THROTTLE_PERCENT = 0.0f;

// Maximum throttle allowed by software
constexpr float MAX_THROTTLE_PERCENT = 100.0f;
constexpr float MIN_THROTTLE_PERCENT = 0.0f;

// Button throttle increment
constexpr float THROTTLE_STEP_PERCENT = 5.0f;

// Smooth ramp
constexpr float THROTTLE_RAMP_UP_PERCENT_PER_SEC = 10.0f;
constexpr float THROTTLE_RAMP_DOWN_PERCENT_PER_SEC = 20.0f;


// ==========================================================
// MOTOR
// ==========================================================

constexpr float MOTOR_KV = 1000.0f;


// ==========================================================
// RPM CONTROL
// ==========================================================

// Minimum allowed target RPM
constexpr float MOTOR_MIN_RPM = 0.0f;

// Maximum normal motor target RPM
constexpr float MOTOR_MAX_RPM = 14000.0f;

// Absolute software safety limit
constexpr float MAX_SAFE_RPM = 14000.0f;

// Target RPM button/dashboard step
constexpr float RPM_TARGET_STEP = 500.0f;

// Default startup target
constexpr float DEFAULT_START_TARGET_RPM = 0.0f;


// ==========================================================
// ACS712-20A CURRENT SENSOR
// ==========================================================

// ACS712-20A sensitivity
// 100 mV/A = 0.100 V/A
constexpr float ACS712_SENSITIVITY = 0.100f;

// Set true because 10k / 20k divider is used
constexpr bool ACS712_USE_DIVIDER = true;

constexpr float ACS712_R1_OHM = 10000.0f;
constexpr float ACS712_R2_OHM = 20000.0f;

constexpr float ACS712_DIVIDER_RATIO =
    ACS712_R2_OHM /
    (ACS712_R1_OHM + ACS712_R2_OHM);

// Zero-current calibration
constexpr uint16_t ACS712_ZERO_CAL_SAMPLES = 500;

// ADC averaging samples
constexpr uint16_t ACS712_READ_SAMPLES = 32;

// Small-current noise suppression
constexpr float ACS712_NOISE_DEADBAND_A = 0.15f;

// Current warning
constexpr float CURRENT_WARNING_A = 12.0f;

// Emergency current cutoff
constexpr float CURRENT_LIMIT_A = 18.0f;

// Valid zero-voltage range
constexpr float ACS712_ZERO_MIN_V = 1.0f;
constexpr float ACS712_ZERO_MAX_V = 4.5f;


// ==========================================================
// BATTERY / VOLTAGE SENSOR
// ==========================================================

constexpr float BATTERY_MIN_VOLTAGE = 9.0f;

constexpr float BATTERY_WARNING_VOLTAGE = 10.5f;

constexpr float BATTERY_NOMINAL_VOLTAGE = 11.1f;

constexpr float BATTERY_MAX_VOLTAGE = 12.6f;

// Common 0-25V voltage sensor module
constexpr float VOLTAGE_SENSOR_DIVIDER_RATIO = 5.0f;

constexpr float MAX_INPUT_VOLTAGE = 25.0f;


// ==========================================================
// BUTTONS
// ==========================================================

constexpr uint32_t BUTTON_DEBOUNCE_MS = 50;


// ==========================================================
// BUZZER
// ==========================================================

constexpr uint16_t BUZZER_WARNING_FREQ = 2000;
constexpr uint16_t BUZZER_ERROR_FREQ = 3000;

constexpr uint32_t BUZZER_WARNING_DURATION_MS = 200;
constexpr uint32_t BUZZER_ERROR_DURATION_MS = 500;


// ==========================================================
// OLED
// ==========================================================

constexpr uint8_t OLED_I2C_ADDRESS = 0x3C;

constexpr uint16_t OLED_WIDTH = 128;
constexpr uint16_t OLED_HEIGHT = 64;


// ==========================================================
// SYSTEM
// ==========================================================

constexpr uint32_t SYSTEM_UPDATE_INTERVAL_MS = 50;

constexpr uint32_t LOGGER_UPDATE_INTERVAL_MS = 1000;

constexpr uint32_t DISPLAY_UPDATE_INTERVAL_MS = 250;

constexpr uint32_t DASHBOARD_UPDATE_INTERVAL_MS = 500;


// ==========================================================
// END
// ==========================================================

#endif // FLOWSENSE_CONSTANTS_H