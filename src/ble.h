#include "Arduino.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <TinyGPS++.h>
#include "Odometer.h"

extern TinyGPSPlus gps;

void ble_begin();
void ble_loop();