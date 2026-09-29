#ifndef FLOWSENSE_CONFIG_H
#define FLOWSENSE_CONFIG_H

#include <Arduino.h>

#include "pins.h"
#include "constants.h"

// ==========================================================
// FlowSense-X
// Global Project Configuration
// ==========================================================

namespace FlowSenseConfig
{

// ==========================================================
// Project Information
// ==========================================================

constexpr const char* PROJECT_NAME =
    "FlowSense-X";


// ==========================================================
// Hardware Features
// ==========================================================

constexpr bool OLED_ENABLED =
    true;

constexpr bool BUTTONS_ENABLED =
    true;

constexpr bool BUZZER_ENABLED =
    false;

constexpr bool CURRENT_SENSOR_ENABLED =
    true;

constexpr bool VOLTAGE_SENSOR_ENABLED =
    true;

constexpr bool ESC_ENABLED =
    true;


// ==========================================================
// Software Features
// ==========================================================

constexpr bool WIFI_ENABLED =
    true;

constexpr bool DASHBOARD_ENABLED =
    true;

constexpr bool LOGGER_ENABLED =
    true;


// ==========================================================
// Safety
// ==========================================================

constexpr bool SAFETY_ENABLED =
    true;


// ==========================================================
// Debug
// ==========================================================

constexpr bool DEBUG_ENABLED =
    true;


// ==========================================================
// WiFi Configuration
// ==========================================================

constexpr const char* WIFI_SSID = "YOUR_WIFI_SSID";
constexpr const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// ==========================================================
// ESP32 Access Point Fallback
// ==========================================================

constexpr const char* AP_SSID = "FlowSense-X";
constexpr const char* AP_PASSWORD = "YOUR_AP_PASSWORD";

// ==========================================================
// Web Server
// ==========================================================

constexpr uint16_t WEB_SERVER_PORT =
    80;

}

// ==========================================================
// End of Configuration
// ==========================================================

#endif // FLOWSENSE_CONFIG_H