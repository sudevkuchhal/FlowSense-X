#ifndef FLOWSENSE_PINS_H
#define FLOWSENSE_PINS_H

#include <Arduino.h>

// ==========================================================
// FlowSense-X
// Final Hardware Pin Mapping
// ESP32 DevKit V1
// ==========================================================


// ==========================================================
// OLED DISPLAY
// ==========================================================

constexpr uint8_t PIN_OLED_SDA = 21;
constexpr uint8_t PIN_OLED_SCL = 22;


// ==========================================================
// ESC / BLDC MOTOR
// ==========================================================
//
// ESP32 GPIO18 -> ESC Signal
//
// ESC GND -> ESP32 GND
//
// ESC Red/BEC wire:
// Do NOT connect directly to ESP32 3.3V.
//

constexpr uint8_t PIN_ESC_SIGNAL = 18;


// ==========================================================
// IR RPM SENSOR
// ==========================================================
//
// IR sensor OUT -> GPIO33
//
// The sensor is used ONLY for RPM measurement.
//
// Black/reflective mark on motor:
//
//      1 detection = 1 revolution
//
// The IR sensor must NEVER directly stop the motor.
// It only generates RPM pulses.
//

// IR sensor OUT -> GPIO33
constexpr uint8_t PIN_IR_RPM_SENSOR = 33;


// ==========================================================
// ACS712 CURRENT SENSOR
// ==========================================================
//
// ACS712 OUT -> GPIO34
//

constexpr uint8_t PIN_CURRENT_SENSOR = 34;


// ==========================================================
// VOLTAGE SENSOR
// ==========================================================
//
// Voltage sensor OUT -> GPIO35
//

constexpr uint8_t PIN_VOLTAGE_SENSOR = 35;


// ==========================================================
// PUSH BUTTONS
// ==========================================================
//
// Button -> GPIO
// Other side -> GND
//
// INPUT_PULLUP is used.
//

constexpr uint8_t PIN_BUTTON_UP = 25;

constexpr uint8_t PIN_BUTTON_DOWN = 26;

constexpr uint8_t PIN_BUTTON_MODE = 27;


// ==========================================================
// BUZZER
// ==========================================================
//
// Buzzer -> GPIO19
//

constexpr uint8_t PIN_BUZZER = 19;




// ==========================================================
// WIFI STATUS LED (onboard blue LED)
// ==========================================================
//
// ESP32 DevKit V1 has a built-in blue LED on GPIO2.
// Solid ON  = connected to your Wi-Fi ("BLDC")
// Blinking  = running its own fallback hotspot (AP mode)
// OFF       = not connected yet / still trying
//
// If your board has no onboard LED on GPIO2, wire an
// external LED (+ resistor) to this pin instead.
//

constexpr uint8_t PIN_WIFI_LED = 2;

#endif // FLOWSENSE_PINS_H