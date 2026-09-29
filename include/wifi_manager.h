#ifndef FLOWSENSE_WIFI_MANAGER_H
#define FLOWSENSE_WIFI_MANAGER_H

#include <Arduino.h>

// ==========================================================
// FlowSense-X
// WiFi Manager Interface
// ==========================================================

bool wifiInit();

void wifiUpdate();

bool wifiIsConnected();

bool wifiIsAccessPoint();

String wifiGetIP();

String wifiGetSSID();

#endif // FLOWSENSE_WIFI_MANAGER_H