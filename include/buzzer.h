#ifndef FLOWSENSE_BUZZER_H
#define FLOWSENSE_BUZZER_H

#include <Arduino.h>

void buzzerInit();

void buzzerOn();
void buzzerOff();

void buzzerBeep(uint32_t durationMs);

bool buzzerIsOn();

#endif