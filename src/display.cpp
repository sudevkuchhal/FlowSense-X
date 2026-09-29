#include "display.h"

#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "pins.h"

// ==========================================================
// FlowSense-X
// Unified OLED Dashboard
// ==========================================================

namespace
{
    // ------------------------------------------------------
    // OLED hardware
    // ------------------------------------------------------

    constexpr uint8_t SCREEN_WIDTH  = 128;
    constexpr uint8_t SCREEN_HEIGHT = 64;
    constexpr uint8_t OLED_ADDRESS  = 0x3C;

    Adafruit_SSD1306 display(
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        &Wire,
        -1
    );

    bool initialized = false;

    // ------------------------------------------------------
    // Common colors
    // ------------------------------------------------------

    constexpr uint16_t OLED_WHITE =
        SSD1306_WHITE;

    // ------------------------------------------------------
    // Helper: draw horizontal separator
    // ------------------------------------------------------

    void drawSeparator(
        int y
    )
    {
        display.drawLine(
            0,
            y,
            127,
            y,
            OLED_WHITE
        );
    }

    // ------------------------------------------------------
    // Helper: draw page header
    // ------------------------------------------------------

    void drawHeader(
        const char* title
    )
    {
        display.setTextSize(1);
        display.setTextColor(OLED_WHITE);

        display.setCursor(
            0,
            0
        );

        display.println(title);

        drawSeparator(10);
    }

    // ------------------------------------------------------
    // Helper: print RPM value
    // ------------------------------------------------------

    void printRpm(
        float rpm
    )
    {
        display.print(
            static_cast<int>(rpm)
        );

        display.print(
            " RPM"
        );
    }

    // ------------------------------------------------------
    // Helper: ESC status
    // ------------------------------------------------------

    const char* escStatus(
        uint16_t pulseUs
    )
    {
        if (pulseUs <= 1000)
        {
            return "STOP";
        }

        if (pulseUs >= 1900)
        {
            return "HIGH";
        }

        return "RUN";
    }
}

// ==========================================================
// OLED INIT
// ==========================================================

bool displayInit()
{
    Wire.begin(
        PIN_OLED_SDA,
        PIN_OLED_SCL
    );

    if (
        !display.begin(
            SSD1306_SWITCHCAPVCC,
            OLED_ADDRESS
        )
    )
    {
        initialized = false;

        return false;
    }

    initialized = true;

    display.clearDisplay();

    display.setTextColor(
        OLED_WHITE
    );

    display.setTextSize(1);

    display.display();

    return true;
}

// ==========================================================
// CLEAR DISPLAY
// ==========================================================

void displayClear()
{
    if (!initialized)
    {
        return;
    }

    display.clearDisplay();

    display.display();
}

// ==========================================================
// BOOT SCREEN
// ==========================================================

void displayShowBoot()
{
    if (!initialized)
    {
        return;
    }

    display.clearDisplay();

    // ------------------------------------------------------
    // Logo
    // ------------------------------------------------------

    display.setTextSize(2);

    display.setCursor(
        8,
        6
    );

    display.println(
        "FlowSense"
    );

    display.setCursor(
        52,
        27
    );

    display.println(
        "X"
    );

    // ------------------------------------------------------
    // Status
    // ------------------------------------------------------

    display.setTextSize(1);

    display.setCursor(
        28,
        49
    );

    display.println(
        "SYSTEM READY"
    );

    display.display();
}

// ==========================================================
// SYSTEM PAGE
// ==========================================================

void showSystemPage(
    float targetRpm,
    bool safetyAllowed,
    const char* systemState
)
{
    drawHeader(
        "SYSTEM STATUS"
    );

    display.setCursor(
        0,
        16
    );

    display.print(
        "STATE : "
    );

    display.println(
        systemState
    );

    display.setCursor(
        0,
        29
    );

    display.print(
        "SAFETY: "
    );

    display.println(
        safetyAllowed
            ? "ENABLED"
            : "BLOCKED"
    );

    display.setCursor(
        0,
        42
    );

    display.print(
        "TARGET: "
    );

    printRpm(
        targetRpm
    );
}

// ==========================================================
// MOTOR PAGE
// ==========================================================

void showMotorPage(
    float targetRpm,
    float commandRpm,
    float measuredRpm
)
{
    drawHeader(
        "MOTOR / RPM"
    );

    // ------------------------------------------------------
    // Actual RPM - primary value
    // ------------------------------------------------------

    display.setCursor(
        0,
        14
    );

    display.print(
        "ACTUAL"
    );

    display.setTextSize(2);

    display.setCursor(
        0,
        25
    );

    display.print(
        static_cast<int>(measuredRpm)
    );

    display.setTextSize(1);

    display.print(
        " RPM"
    );

    // ------------------------------------------------------
    // Target + command
    // ------------------------------------------------------

    display.setCursor(
        0,
        46
    );

    display.print(
        "T:"
    );

    display.print(
        static_cast<int>(targetRpm)
    );

    display.print(
        " C:"
    );

    display.print(
        static_cast<int>(commandRpm)
    );

    display.print(
        " RPM"
    );
}

// ==========================================================
// CURRENT PAGE
// ==========================================================

void showCurrentPage(
    float currentA
)
{
    drawHeader(
        "MOTOR CURRENT"
    );

    display.setTextSize(2);

    display.setCursor(
        0,
        20
    );

    display.print(
        currentA,
        2
    );

    display.setTextSize(1);

    display.setCursor(
        101,
        27
    );

    display.println(
        "A"
    );

    display.setCursor(
        0,
        50
    );

    display.println(
        "ACS712-20A"
    );
}

// ==========================================================
// VOLTAGE PAGE
// ==========================================================

void showVoltagePage(
    float voltageV
)
{
    drawHeader(
        "BATTERY VOLTAGE"
    );

    display.setTextSize(2);

    display.setCursor(
        0,
        20
    );

    display.print(
        voltageV,
        2
    );

    display.setTextSize(1);

    display.setCursor(
        101,
        27
    );

    display.println(
        "V"
    );

    display.setCursor(
        0,
        50
    );

    if (voltageV < 10.5f)
    {
        display.println(
            "BATTERY LOW"
        );
    }
    else
    {
        display.println(
            "BATTERY OK"
        );
    }
}

// ==========================================================
// ESC PAGE
// ==========================================================

void showEscPage(
    uint16_t escPulseUs
)
{
    drawHeader(
        "ESC CONTROL"
    );

    display.setCursor(
        0,
        17
    );

    display.print(
        "PULSE:"
    );

    display.setTextSize(2);

    display.setCursor(
        0,
        28
    );

    display.print(
        escPulseUs
    );

    display.setTextSize(1);

    display.print(
        " us"
    );

    display.setCursor(
        0,
        51
    );

    display.print(
        "STATUS: "
    );

    display.println(
        escStatus(
            escPulseUs
        )
    );
}

// ==========================================================
// SAFETY PAGE
// ==========================================================

void showSafetyPage(
    bool safetyAllowed,
    const char* systemState
)
{
    drawHeader(
        "SAFETY STATUS"
    );

    // ------------------------------------------------------
    // Safety state
    // ------------------------------------------------------

    display.setTextSize(2);

    display.setCursor(
        4,
        20
    );

    if (safetyAllowed)
    {
        display.println(
            "SAFE"
        );
    }
    else
    {
        display.println(
            "BLOCKED"
        );
    }

    display.setTextSize(1);

    // ------------------------------------------------------
    // System state
    // ------------------------------------------------------

    display.setCursor(
        0,
        46
    );

    display.print(
        "STATE: "
    );

    display.println(
        systemState
    );
}

// ==========================================================
// MAIN DISPLAY FUNCTION
// ==========================================================

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
)
{
    if (!initialized)
    {
        return;
    }

    // ------------------------------------------------------
    // Clear previous frame
    // ------------------------------------------------------

    display.clearDisplay();

    display.setTextColor(
        OLED_WHITE
    );

    // ------------------------------------------------------
    // Render selected page
    // ------------------------------------------------------

    switch (page)
    {
        case DisplayPage::System:

            showSystemPage(
                targetRpm,
                safetyAllowed,
                systemState
            );

            break;

        case DisplayPage::Motor:

            showMotorPage(
                targetRpm,
                commandRpm,
                measuredRpm
            );

            break;

        case DisplayPage::Current:

            showCurrentPage(
                currentA
            );

            break;

        case DisplayPage::Voltage:

            showVoltagePage(
                voltageV
            );

            break;

        case DisplayPage::ESC:

            showEscPage(
                escPulseUs
            );

            break;

        case DisplayPage::Safety:

            showSafetyPage(
                safetyAllowed,
                systemState
            );

            break;

        default:

            drawHeader(
                "FLOWSENSE-X"
            );

            display.setCursor(
                0,
                25
            );

            display.println(
                "DISPLAY ERROR"
            );

            break;
    }

    // ------------------------------------------------------
    // Push framebuffer to OLED
    // ------------------------------------------------------

    display.display();
}