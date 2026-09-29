#ifndef FLOWSENSE_BUTTONS_H
#define FLOWSENSE_BUTTONS_H

#include <Arduino.h>

void buttonsInit();

bool buttonUpPressed();
bool buttonDownPressed();
bool buttonModePressed();

#endif