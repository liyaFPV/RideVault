#ifndef ODOMETER_H
#define ODOMETER_H

#include <Arduino.h>

// Объявления переменных
extern double ode_km;
extern double ode1_km;
extern double ode2_km;
extern double ode3_km;

extern double avgSpeed;
extern float maxSpeed;

void GetMaxSpeed();
void GetAgvSpeed();
void setupOdometer();
void saveOdometer();
void updateAllOdometer();

#endif