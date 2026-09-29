#include "wifi_manager.h"

#include <WiFi.h>

#include "config.h"
#include "pins.h"

// ==========================================================
// FlowSense-X
// WiFi Manager Implementation
// ==========================================================

namespace
{
    bool connected = false;
    bool accessPointMode = false;
}

// ==========================================================
// WiFi Status LED (onboard blue LED, GPIO2)
//
// Solid ON  = connected to configured Wi-Fi
// Blinking  = running fallback Access Point
// OFF       = not connected
// ==========================================================

namespace
{
    void wifiLedSetup()
    {
        pinMode(PIN_WIFI_LED, OUTPUT);
        digitalWrite(PIN_WIFI_LED, LOW);
    }

    void wifiLedSolid(bool on)
    {
        digitalWrite(PIN_WIFI_LED, on ? HIGH : LOW);
    }

    void wifiLedBlink()
    {
        // Slow blink (500 ms on/off) to show "own hotspot" mode.
        const bool ledOn = (millis() / 500) % 2 == 0;
        digitalWrite(PIN_WIFI_LED, ledOn ? HIGH : LOW);
    }
}

// ==========================================================
// WiFi Initialization
// ==========================================================

bool wifiInit()
{
    Serial.println();
    Serial.println("[WIFI] Starting...");

    connected = false;
    accessPointMode = false;

    wifiLedSetup();

    // ------------------------------------------------------
    // Try Station Mode
    // ------------------------------------------------------

    WiFi.mode(WIFI_STA);

    WiFi.begin(
        FlowSenseConfig::WIFI_SSID,
        FlowSenseConfig::WIFI_PASSWORD
    );

    const unsigned long startTime =
        millis();

    while (
        WiFi.status() != WL_CONNECTED &&
        millis() - startTime < 10000UL
    )
    {
        delay(500);

        Serial.print(".");
    }

    // ------------------------------------------------------
    // Router connection successful
    // ------------------------------------------------------

    if (WiFi.status() == WL_CONNECTED)
    {
        connected = true;
        accessPointMode = false;

        wifiLedSolid(true);

        Serial.println();
        Serial.println("[WIFI] Connected");

        Serial.print("[WIFI] SSID: ");
        Serial.println(
            WiFi.SSID()
        );

        Serial.print("[WIFI] IP: ");
        Serial.println(
            WiFi.localIP()
        );

        return true;
    }

    // ======================================================
    // Fallback Access Point
    // ======================================================

    Serial.println();
    Serial.println(
        "[WIFI] Router connection failed"
    );

    Serial.println(
        "[WIFI] Starting Access Point"
    );

    WiFi.disconnect(true);

    delay(500);

    WiFi.mode(WIFI_AP);

    const bool apStarted =
        WiFi.softAP(
            FlowSenseConfig::AP_SSID,
            FlowSenseConfig::AP_PASSWORD
        );

    if (!apStarted)
    {
        connected = false;
        accessPointMode = false;

        wifiLedSolid(false);

        Serial.println(
            "[WIFI] AP FAILED"
        );

        return false;
    }

    connected = true;
    accessPointMode = true;

    Serial.println(
        "[WIFI] Access Point Started"
    );

    Serial.print(
        "[WIFI] AP SSID: "
    );

    Serial.println(
        FlowSenseConfig::AP_SSID
    );

    Serial.print(
        "[WIFI] AP IP: "
    );

    Serial.println(
        WiFi.softAPIP()
    );

    return true;
}

// ==========================================================
// WiFi Runtime Update
// ==========================================================

void wifiUpdate()
{
    // AP fallback mode: slow blink to show "own hotspot" is active.
    // STA mode: LED is already solid ON (set once in wifiInit()).
    if (accessPointMode)
    {
        wifiLedBlink();
    }

    // Reserved for future:
    //
    // - automatic reconnection
    // - connection monitoring
    // - AP/client monitoring
}

// ==========================================================
// Connection Status
// ==========================================================

bool wifiIsConnected()
{
    return connected;
}

// ==========================================================
// Access Point Status
// ==========================================================

bool wifiIsAccessPoint()
{
    return accessPointMode;
}

// ==========================================================
// Current IP Address
// ==========================================================

String wifiGetIP()
{
    if (accessPointMode)
    {
        return WiFi.softAPIP().toString();
    }

    return WiFi.localIP().toString();
}

// ==========================================================
// Current SSID
// ==========================================================

String wifiGetSSID()
{
    if (accessPointMode)
    {
        return String(
            FlowSenseConfig::AP_SSID
        );
    }

    return WiFi.SSID();
}