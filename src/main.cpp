#include <Arduino.h>

#include "config.h"
#include "constants.h"
#include "version.h"

#include "esc.h"
#include "motor.h"
#include "motion.h"
#include "system.h"
#include "rpm_sensor.h"
#include "auto_peak.h"

#include "current_sensor.h"
#include "voltage_sensor.h"

#include "safety.h"
#include "events.h"

#include "logger.h"

#include "wifi_manager.h"
#include "web_dashboard.h"

#include "display.h"
#include "buttons.h"
#include "buzzer.h"
#include "dashboard.h"

// ==========================================================
// FlowSense-X
// Main Application
//
// IMPORTANT:
// RPM is measured only from the physical IR sensor on GPIO33.
// No RPM is estimated from ESC PWM.
//
// Startup policy:
// - Sensor failure -> WARNING, continue
// - ESC failure    -> WARNING, continue Wi-Fi/dashboard
// - Motor remains stopped until commanded
// ==========================================================

namespace
{
    uint32_t lastUpdateMs = 0;

    constexpr uint32_t CONTROL_INTERVAL_MS = 50;

    bool motorAvailable = false;
    bool currentSensorAvailable = false;
    bool voltageSensorAvailable = false;
    bool rpmSensorAvailable = false;
}

// ==========================================================
// SETUP
// ==========================================================

void setup()
{
    Serial.begin(SERIAL_BAUD_RATE);

    delay(STARTUP_DELAY_MS);

    Serial.println();
    Serial.println("========================================");
    Serial.println("             FlowSense-X");
    Serial.println("      Intelligent Motion Controller");
    Serial.println("========================================");

    Serial.print("Version : ");
    Serial.println(FLOWSENSE_VERSION);

    Serial.print("Codename: ");
    Serial.println(FLOWSENSE_CODENAME);

    Serial.println();

    // ======================================================
    // OLED
    // ======================================================

    if (FlowSenseConfig::OLED_ENABLED)
    {
        if (displayInit())
        {
            displayShowBoot();
            Serial.println("[INIT] OLED OK");
        }
        else
        {
            Serial.println("[WARN] OLED initialization FAILED");
        }
    }

    // ======================================================
    // BUTTONS
    // ======================================================

    if (FlowSenseConfig::BUTTONS_ENABLED)
    {
        buttonsInit();
        Serial.println("[INIT] Buttons OK");
    }

    // ======================================================
    // BUZZER
    // ======================================================

    if (FlowSenseConfig::BUZZER_ENABLED)
    {
        buzzerInit();
        Serial.println("[INIT] Buzzer OK");
    }

    // ======================================================
    // LOCAL DASHBOARD
    // ======================================================

    if (
        FlowSenseConfig::OLED_ENABLED ||
        FlowSenseConfig::BUTTONS_ENABLED
    )
    {
        dashboardInit();
    }

    // ======================================================
    // SENSOR INITIALIZATION
    // ======================================================

    Serial.println("[INIT] Initializing sensors...");

    // ------------------------------------------------------
    // CURRENT SENSOR
    // ------------------------------------------------------

    if (FlowSenseConfig::CURRENT_SENSOR_ENABLED)
    {
        if (currentSensorInit())
        {
            currentSensorAvailable = true;
            Serial.println("[INIT] Current sensor OK");
        }
        else
        {
            currentSensorAvailable = false;

            Serial.println("[WARN] Current sensor FAILED");
            Serial.println(
                "[WARN] Continuing without valid current sensor"
            );
        }
    }

    // ------------------------------------------------------
    // IR RPM SENSOR
    // ------------------------------------------------------

    if (rpmSensorInit())
    {
        rpmSensorAvailable = true;
        Serial.println("[INIT] IR RPM sensor OK on GPIO33");
    }
    else
    {
        rpmSensorAvailable = false;
        Serial.println("[WARN] IR RPM sensor FAILED");
    }

    // ------------------------------------------------------
    // VOLTAGE SENSOR
    // ------------------------------------------------------

    if (FlowSenseConfig::VOLTAGE_SENSOR_ENABLED)
    {
        if (voltageSensorInit())
        {
            voltageSensorAvailable = true;
            Serial.println("[INIT] Voltage sensor OK");
        }
        else
        {
            voltageSensorAvailable = false;

            Serial.println("[WARN] Voltage sensor FAILED");
            Serial.println(
                "[WARN] Continuing with voltage unavailable"
            );
        }
    }

    // ======================================================
    // ESC / MOTOR
    // ======================================================

    if (FlowSenseConfig::ESC_ENABLED)
    {
        Serial.println("[INIT] Initializing ESC...");

        if (motorInit())
        {
            motorAvailable = true;

            Serial.println("[INIT] ESC attached");
            Serial.println("[INIT] Motor controller OK");

            // Keep motor stopped after initialization
            motorStop();
        }
        else
        {
            motorAvailable = false;

            Serial.println("[WARN] Motor initialization FAILED");
            Serial.println(
                "[WARN] Continuing startup for Wi-Fi/dashboard"
            );
            Serial.println(
                "[WARN] Motor output will remain disabled"
            );
        }
    }

    // ======================================================
    // MOTION
    // ======================================================

    Serial.println("[INIT] Initializing motion...");
    motionInit();

    // ======================================================
    // SAFETY
    // ======================================================

    Serial.println("[INIT] Initializing safety...");
    safetyInit();

    // ======================================================
    // EVENTS
    // ======================================================

    Serial.println("[INIT] Initializing events...");
    eventsInit();

    // ======================================================
    // SYSTEM
    // ======================================================

    Serial.println("[INIT] Initializing system...");
    systemInit();

    // ======================================================
    // LOGGER
    // ======================================================

    if (FlowSenseConfig::LOGGER_ENABLED)
    {
        loggerInit();
        Serial.println("[INIT] Logger OK");
    }

    // ======================================================
    // WIFI
    // ======================================================

    Serial.println();
    Serial.println("[INIT] Initializing Wi-Fi...");

    if (FlowSenseConfig::WIFI_ENABLED)
    {
        if (wifiInit())
        {
            Serial.println("[INIT] Wi-Fi OK");

            Serial.print("[WIFI] SSID: ");
            Serial.println(wifiGetSSID());

            Serial.print("[WIFI] IP: ");
            Serial.println(wifiGetIP());
        }
        else
        {
            Serial.println("[WARN] Wi-Fi initialization failed");
            Serial.println(
                "[WARN] Dashboard may be unavailable"
            );
        }
    }
    else
    {
        Serial.println("[WIFI] Wi-Fi disabled in configuration");
    }

    // ======================================================
    // WEB DASHBOARD
    // ======================================================

    if (
        FlowSenseConfig::DASHBOARD_ENABLED &&
        wifiIsConnected()
    )
    {
        webDashboardInit();
        Serial.println("[INIT] Dashboard OK");
    }
    else if (FlowSenseConfig::DASHBOARD_ENABLED)
    {
        Serial.println(
            "[WARN] Dashboard not started because Wi-Fi is unavailable"
        );
    }

    // ======================================================
    // INITIAL SYSTEM STATE
    // ======================================================

    systemSetTargetRpm(0.0f);

    systemSetMeasuredRpm(0.0f);
    autoPeakInit();

    // Initial electrical values
    systemSetCurrent(0.0f);
    systemSetVoltage(0.0f);

    systemSetActualEscPulse(ESC_MIN_US);

    // ======================================================
    // MOTOR SAFETY
    // ======================================================

    if (motorAvailable)
    {
        motorStop();
    }

    lastUpdateMs = millis();

    // ======================================================
    // READY
    // ======================================================

    Serial.println();
    Serial.println("========================================");
    Serial.println("          FlowSense-X READY");
    Serial.println("========================================");

    Serial.print("[STATUS] Motor: ");
    Serial.println(
        motorAvailable ? "AVAILABLE" : "UNAVAILABLE"
    );

    Serial.print("[STATUS] Current Sensor: ");
    Serial.println(
        currentSensorAvailable ? "AVAILABLE" : "UNAVAILABLE"
    );

    Serial.print("[STATUS] Voltage Sensor: ");
    Serial.println(
        voltageSensorAvailable ? "AVAILABLE" : "UNAVAILABLE"
    );

    Serial.print("[STATUS] Wi-Fi: ");
    Serial.println(
        wifiIsConnected() ? "CONNECTED" : "NOT CONNECTED"
    );

    Serial.println();
}

// ==========================================================
// LOOP
// ==========================================================

void loop()
{
    const uint32_t now = millis();

    // ======================================================
    // WIFI UPDATE
    // ======================================================

    if (FlowSenseConfig::WIFI_ENABLED)
    {
        wifiUpdate();
    }

    // Process local button controls continuously.
    if (FlowSenseConfig::BUTTONS_ENABLED)
    {
        dashboardHandleInputs();
    }

    // ======================================================
    // WEB DASHBOARD
    // ======================================================

    if (
        FlowSenseConfig::DASHBOARD_ENABLED &&
        wifiIsConnected()
    )
    {
        webDashboardUpdate();
    }

    // ======================================================
    // LOCAL DASHBOARD
    // ======================================================

    dashboardUpdate();

    // ======================================================
    // CONTROL INTERVAL
    // ======================================================

    if ((now - lastUpdateMs) < CONTROL_INTERVAL_MS)
    {
        return;
    }

    const float dtSeconds =
        static_cast<float>(now - lastUpdateMs) / 1000.0f;

    lastUpdateMs = now;

    // ======================================================
    // CURRENT SENSOR
    // ======================================================

    if (
        FlowSenseConfig::CURRENT_SENSOR_ENABLED &&
        currentSensorAvailable
    )
    {
        currentSensorUpdate();

        const float currentA = currentSensorReadCurrent();

        systemSetCurrent(currentA);
    }
    else
    {
        systemSetCurrent(0.0f);
    }

    // ======================================================
    // VOLTAGE SENSOR
    // ======================================================

    if (
        FlowSenseConfig::VOLTAGE_SENSOR_ENABLED &&
        voltageSensorAvailable
    )
    {
        voltageSensorUpdate();

        const float voltageV = voltageSensorReadVoltage();

        systemSetVoltage(voltageV);
    }
    else
    {
        systemSetVoltage(0.0f);
    }

    // ======================================================
    // IR RPM SENSOR
    // ======================================================

    if (rpmSensorAvailable)
    {
        rpmSensorUpdate();
        systemSetMeasuredRpm(rpmSensorGetRpm());
    }
    else
    {
        systemSetMeasuredRpm(0.0f);
    }

    // ======================================================
    // AUTO PEAK
    // ======================================================

    autoPeakUpdate(dtSeconds);

    // ======================================================
    // SYSTEM UPDATE
    // ======================================================
    //
    // Project API requires a float argument.
    // 0.0f = no external measured-RPM feedback.
    // ======================================================

    systemUpdate(dtSeconds);

    // ======================================================
    // MOTOR OUTPUT
    // ======================================================

    if (motorAvailable)
    {
        const SystemStatus status = systemGetStatus();

        if (status.escPulseUs <= ESC_MIN_US || status.targetRpm <= MOTOR_MIN_RPM)
        {
            motorStop();
        }
        else
        {
            if (!motorIsEnabled())
            {
                motorStart();
            }

            motorSetPulse(status.escPulseUs);
        }
    }
    else
    {
        // Motor unavailable.
        // Do not send motor output.
        systemSetActualEscPulse(ESC_MIN_US);
    }

    // ======================================================
    // LOGGER
    // ======================================================

    if (FlowSenseConfig::LOGGER_ENABLED)
    {
        const SystemStatus status = systemGetStatus();

        loggerUpdate(status);
    }

    // ======================================================
    // SAFETY
    // ======================================================
    //
    // Safety emergency function is not exposed by the
    // current safety.h API, so safety processing remains
    // inside systemUpdate().
    // ======================================================

    if (!motorAvailable)
    {
        systemSetActualEscPulse(ESC_MIN_US);
    }
}