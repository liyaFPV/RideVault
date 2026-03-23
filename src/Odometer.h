#ifndef ODOMETER_H
#define ODOMETER_H

#include <Arduino.h>

// Объявления переменных
extern double ode;
extern double ode1;
extern double ode2;
extern double ode3;

extern double avgSpeed;
extern float maxSpeed;

void GetMaxSpeed();
void GetAgvSpeed();
void setupOdometer();
void saveOdometer();
void updateAllOdometer();

#endif