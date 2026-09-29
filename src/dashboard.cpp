#include "dashboard.h"

#include <Arduino.h>

#include "buttons.h"
#include "buzzer.h"
#include "constants.h"

// ==========================================================
// FlowSense-X
// Dashboard Implementation
// ==========================================================

namespace
{
    // ------------------------------------------------------
    // Dashboard state
    // ------------------------------------------------------

    DisplayPage currentPage =
        DisplayPage::System;

    uint32_t lastDisplayUpdateMs = 0;

    // ------------------------------------------------------
    // Page count
    //
    // IMPORTANT:
    // Keep this synchronized with DisplayPage enum.
    // ------------------------------------------------------

    constexpr uint8_t DISPLAY_PAGE_COUNT = 6;

    // ------------------------------------------------------
    // Convert system state to display text
    // ------------------------------------------------------

    const char* systemStateToString(
        SystemState state
    )
    {
        switch (state)
        {
            case SystemState::Idle:
                return "IDLE";

            case SystemState::Running:
                return "RUN";

            case SystemState::Warning:
                return "WARN";

            case SystemState::Fault:
                return "FAULT";

            case SystemState::EmergencyStop:
                return "E-STOP";

            default:
                return "UNKNOWN";
        }
    }

    // ------------------------------------------------------
    // Increase target RPM
    // ------------------------------------------------------

    float increaseTarget(
        float currentTarget
    )
    {
        const float newTarget =
            currentTarget +
            RPM_TARGET_STEP;

        return constrain(
            newTarget,
            MOTOR_MIN_RPM,
            MAX_SAFE_RPM
        );
    }

    // ------------------------------------------------------
    // Decrease target RPM
    // ------------------------------------------------------

    float decreaseTarget(
        float currentTarget
    )
    {
        const float newTarget =
            currentTarget -
            RPM_TARGET_STEP;

        return constrain(
            newTarget,
            MOTOR_MIN_RPM,
            MAX_SAFE_RPM
        );
    }

    // ------------------------------------------------------
    // Move to next display page
    // ------------------------------------------------------

    void nextPage()
    {
        const uint8_t currentIndex =
            static_cast<uint8_t>(
                currentPage
            );

        currentPage =
            static_cast<DisplayPage>(
                (
                    currentIndex + 1U
                ) %
                DISPLAY_PAGE_COUNT
            );
    }

    // ------------------------------------------------------
    // Safety pages have priority
    // ------------------------------------------------------

    DisplayPage getEffectivePage(
        const SystemStatus& status
    )
    {
        if (
            status.state ==
                SystemState::Fault ||
            status.state ==
                SystemState::EmergencyStop
        )
        {
            return DisplayPage::Safety;
        }

        return currentPage;
    }

    // ------------------------------------------------------
    // Emergency buzzer
    // ------------------------------------------------------

    void updateEmergencyBeep(
        uint32_t nowMs
    )
    {
        static uint32_t lastBeepMs = 0;

        if (
            nowMs - lastBeepMs >=
            1000UL
        )
        {
            lastBeepMs = nowMs;

            buzzerBeep(250);
        }
    }

    // ------------------------------------------------------
    // Fault buzzer
    // ------------------------------------------------------

    void updateFaultBeep(
        uint32_t nowMs
    )
    {
        static uint32_t lastBeepMs = 0;

        if (
            nowMs - lastBeepMs >=
            1500UL
        )
        {
            lastBeepMs = nowMs;

            buzzerBeep(180);
        }
    }

    // ------------------------------------------------------
    // Warning buzzer
    // ------------------------------------------------------

    void updateWarningBeep(
        uint32_t nowMs
    )
    {
        static uint32_t lastBeepMs = 0;

        if (
            nowMs - lastBeepMs >=
            2500UL
        )
        {
            lastBeepMs = nowMs;

            buzzerBeep(100);
        }
    }

    // ------------------------------------------------------
    // Update status indication
    // ------------------------------------------------------

    void updateStatusIndication(
        const SystemStatus& status,
        uint32_t nowMs
    )
    {
        switch (status.state)
        {
            case SystemState::EmergencyStop:
                updateEmergencyBeep(nowMs);
                break;

            case SystemState::Fault:
                updateFaultBeep(nowMs);
                break;

            case SystemState::Warning:
                updateWarningBeep(nowMs);
                break;

            default:
                break;
        }
    }
}

// ==========================================================
// Initialize dashboard
// ==========================================================

void dashboardInit()
{
    currentPage =
        DisplayPage::System;

    lastDisplayUpdateMs =
        millis();
}

// ==========================================================
// Handle buttons
// ==========================================================

void dashboardHandleInputs()
{
    // ------------------------------------------------------
    // UP
    // ------------------------------------------------------

    if (buttonUpPressed())
    {
        const SystemStatus status =
            systemGetStatus();

        const float newTarget =
            increaseTarget(
                status.targetRpm
            );

        systemSetTargetRpm(
            newTarget
        );

        buzzerBeep(60);

        const SystemStatus updated =
            systemGetStatus();

        Serial.print(
            "[DASHBOARD] UP | TARGET="
        );

        Serial.print(
            updated.targetRpm,
            0
        );

        Serial.println(" RPM");
    }

    // ------------------------------------------------------
    // DOWN
    // ------------------------------------------------------

    if (buttonDownPressed())
    {
        const SystemStatus status =
            systemGetStatus();

        const float newTarget =
            decreaseTarget(
                status.targetRpm
            );

        systemSetTargetRpm(
            newTarget
        );

        buzzerBeep(60);

        const SystemStatus updated =
            systemGetStatus();

        Serial.print(
            "[DASHBOARD] DOWN | TARGET="
        );

        Serial.print(
            updated.targetRpm,
            0
        );

        Serial.println(" RPM");
    }

    // ------------------------------------------------------
    // MODE
    // ------------------------------------------------------

    if (buttonModePressed())
    {
        nextPage();

        buzzerBeep(120);

        Serial.print(
            "[DASHBOARD] MODE | PAGE="
        );

        Serial.println(
            static_cast<uint8_t>(
                currentPage
            )
        );
    }
}

// ==========================================================
// Dashboard update
// ==========================================================

void dashboardUpdate()
{
    const uint32_t nowMs =
        millis();

    // ------------------------------------------------------
    // Display update rate
    // ------------------------------------------------------

    if (
        nowMs -
        lastDisplayUpdateMs <
        DISPLAY_UPDATE_INTERVAL_MS
    )
    {
        return;
    }

    lastDisplayUpdateMs =
        nowMs;

    // ------------------------------------------------------
    // Get complete system snapshot
    // ------------------------------------------------------

    const SystemStatus status =
        systemGetStatus();

    // ------------------------------------------------------
    // Safety / warning indication
    // ------------------------------------------------------

    updateStatusIndication(
        status,
        nowMs
    );

    // ------------------------------------------------------
    // Select display page
    // ------------------------------------------------------

    const DisplayPage page =
        getEffectivePage(
            status
        );

    // ------------------------------------------------------
    // Update OLED
    // ------------------------------------------------------

    displayShowPage(
        page,

        status.targetRpm,
        status.commandRpm,
        status.measuredRpm,

        status.currentA,
        status.voltageV,

        status.escPulseUs,

        status.safetyAllowed,

        systemStateToString(
            status.state
        )
    );
}

// ==========================================================
// Get current dashboard page
// ==========================================================

DisplayPage dashboardGetPage()
{
    return currentPage;
}