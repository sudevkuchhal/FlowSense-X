#ifndef FLOWSENSE_LOGGER_H
#define FLOWSENSE_LOGGER_H

#include <Arduino.h>

#include "system.h"

// ==========================================================
// FlowSense-X
// Runtime Data Logger Interface
// ==========================================================

void loggerInit();

void loggerUpdate(
    const SystemStatus& status
);

void loggerPrintHeader();

#endif // FLOWSENSE_LOGGER_H