#include <Arduino.h>
#include "config.h"
#include <TinyGPS++.h>
#include <SPI.h>
#include <SD.h>
#include <ArduinoJson.h>

extern TinyGPSPlus gps;
double ode,ode1,ode2,ode3;
float maxSpeed, avgSpeed;

void updateAllOdometer();
void setupOdometer();