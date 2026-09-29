#include "buzzer.h"
#include "config.h"

void buzzerInit()
{
    if (!FlowSenseConfig::BUZZER_ENABLED) return;
}

void buzzerOn()
{
    if (!FlowSenseConfig::BUZZER_ENABLED) return;
}

void buzzerOff()
{
    if (!FlowSenseConfig::BUZZER_ENABLED) return;
}

void buzzerBeep(uint32_t durationMs)
{
    (void)durationMs;
    if (!FlowSenseConfig::BUZZER_ENABLED) return;
}

bool buzzerIsOn()
{
    return false;
}
