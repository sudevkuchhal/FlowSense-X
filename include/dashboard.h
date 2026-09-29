#ifndef FLOWSENSE_DASHBOARD_H
#define FLOWSENSE_DASHBOARD_H

#include <Arduino.h>

#include "display.h"
#include "system.h"

// ==========================================================
// FlowSense-X
// Dashboard Interface
//
// Responsibilities:
// - UP    : Increase target RPM
// - DOWN  : Decrease target RPM
// - MODE  : Change display page
// - Display system status
// - Provide visual/audio status indication
//
// Safety/motor control remains inside system/motor modules.
// ==========================================================

void dashboardInit();

void dashboardHandleInputs();

void dashboardUpdate();

DisplayPage dashboardGetPage();

#endif // FLOWSENSE_DASHBOARD_H