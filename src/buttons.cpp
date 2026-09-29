#include "buttons.h"

#include "pins.h"
#include "constants.h"

namespace
{
bool lastUpState = HIGH;
bool lastDownState = HIGH;
bool lastModeState = HIGH;

unsigned long lastUpTime = 0;
unsigned long lastDownTime = 0;
unsigned long lastModeTime = 0;

bool checkButton(
    uint8_t pin,
    bool& lastState,
    unsigned long& lastTime
)
{
    const bool currentState = digitalRead(pin);

    if (currentState != lastState)
    {
        lastState = currentState;

        if (currentState == LOW)
        {
            const unsigned long now = millis();

            if (now - lastTime >= BUTTON_DEBOUNCE_MS)
            {
                lastTime = now;
                return true;
            }
        }
    }

    return false;
}
}

void buttonsInit()
{
    pinMode(PIN_BUTTON_UP, INPUT_PULLUP);
    pinMode(PIN_BUTTON_DOWN, INPUT_PULLUP);
    pinMode(PIN_BUTTON_MODE, INPUT_PULLUP);

    lastUpState = digitalRead(PIN_BUTTON_UP);
    lastDownState = digitalRead(PIN_BUTTON_DOWN);
    lastModeState = digitalRead(PIN_BUTTON_MODE);
}

bool buttonUpPressed()
{
    return checkButton(
        PIN_BUTTON_UP,
        lastUpState,
        lastUpTime
    );
}

bool buttonDownPressed()
{
    return checkButton(
        PIN_BUTTON_DOWN,
        lastDownState,
        lastDownTime
    );
}

bool buttonModePressed()
{
    return checkButton(
        PIN_BUTTON_MODE,
        lastModeState,
        lastModeTime
    );
}