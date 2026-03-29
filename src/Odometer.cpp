#include "Odometer.h"
#include <SD.h>
#include <ArduinoJson.h>
#include <TinyGPS++.h>

double ode_km  = 0;
double ode1_km = 0;
double ode2_km = 0;
double ode3_km = 0;

extern TinyGPSPlus gps;

#define ODO_COUNT 5

// индексы одометров
#define ODO_SYSTEM 0
#define ODO_TRIP 1
#define ODO_TRIP1 2
#define ODO_TRIP2 3
#define ODO_USER3 4

double odometer[ODO_COUNT] = {0};

double lastLat = 0;
double lastLng = 0;

float maxSpeed = 0;
double avgSpeed = 0;

unsigned long startTime = 0;
unsigned long lastSave = 0;

void updateOdometers()
{
    if (!gps.location.isUpdated())
        return;

    double lat = gps.location.lat();
    double lng = gps.location.lng();

    if (lastLat != 0 && lastLng != 0)
    {
        double distance = TinyGPSPlus::distanceBetween(
            lastLat, lastLng,
            lat, lng);

        if (distance > 0 && distance < 1000)
        {
            double km = distance / 1000.0;

            for (int i = 0; i < ODO_COUNT; i++)
            {
                odometer[i] += km;
            }
        }
    }

    lastLat = lat;
    lastLng = lng;
}

void resetOdometer(int id)
{
    if (id >= 0 && id < ODO_COUNT)
    {
        odometer[id] = 0;
    }
}

double getOdometer(int id)
{
    if (id >= 0 && id < ODO_COUNT)
        return odometer[id];

    return 0;
}

void getMaxSpeed()
{
    float speed = gps.speed.kmph();

    if (speed > maxSpeed)
        maxSpeed = speed;
}

void getAvgSpeed()
{
    double time_s = (millis() - startTime) / 1000.0;

    if (time_s > 0)
        avgSpeed = (odometer[ODO_SYSTEM] / time_s) * 3600.0;
}

void setupOdometer()
{
    startTime = millis();

    if (!SD.exists("/Odometer.json"))
    {
        File file = SD.open("/Odometer.json", FILE_WRITE);

        if (file)
        {
            StaticJsonDocument<256> doc;

            doc["odo0"] = 0;
            doc["odo1"] = 0;
            doc["odo2"] = 0;
            doc["odo3"] = 0;
            doc["odo4"] = 0;

            serializeJsonPretty(doc, file);
            file.close();
        }
    }

    File file = SD.open("/Odometer.json");

    if (!file)
    {
        Serial.println("Ошибка открытия Odometer.json");
        return;
    }

    StaticJsonDocument<256> doc;

    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error)
    {
        Serial.println("Ошибка JSON");
        return;
    }

    for (int i = 0; i < ODO_COUNT; i++)
    {
        String key = "odo" + String(i);
        odometer[i] = doc[key] | 0;
    }

    Serial.println("Odometers loaded");
}

void saveOdometer()
{
    File file = SD.open("/Odometer.json", FILE_WRITE);

    if (!file)
    {
        Serial.println("Ошибка записи файла");
        return;
    }

    StaticJsonDocument<256> doc;

    for (int i = 0; i < ODO_COUNT; i++)
    {
        String key = "odo" + String(i);
        doc[key] = odometer[i];
    }

    serializeJsonPretty(doc, file);

    file.close();

    Serial.println("Odometers saved");
}

void updateAllOdometer()
{
    updateOdometers();

    getMaxSpeed();
    getAvgSpeed();

    if (millis() - lastSave > 1000)
    {
        saveOdometer();
        lastSave = millis();
    }
    ode_km  = odometer[1];
    ode1_km = odometer[2];
    ode2_km = odometer[3];
    ode3_km = odometer[4];
}