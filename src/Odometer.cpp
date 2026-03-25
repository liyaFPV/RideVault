#include "Odometer.h"
#include <SD.h>
#include <ArduinoJson.h>
#include <TinyGPS++.h>

// Определения переменных
double ode_km = 0;
double ode1_km = 0;
double ode2_km = 0;
double ode3_km = 0;

double avgSpeed = 0;
float maxSpeed = 0;

int time_start = millis();
int time_ms;
extern TinyGPSPlus gps;

// Остальной код функций, который у тебя есть
double lastLat0,lastLng0 = 0;
double ode0_km;
void updateOdometer0(){
    if (gps.location.isUpdated())
    {
        double lat0 = gps.location.lat();
        double lng0 = gps.location.lng();
        if (lastLat0 != 0 && lastLng0 != 0)
        {
            double distance0 = TinyGPSPlus::distanceBetween(
                lastLat0, lastLng0,
                lat0, lng0
            );
            if (distance0 > 2 && distance0 < 1000)
            {
                ode0_km += distance0 / 1000.0;
            }
        }
        lastLat0 = lat0;
        lastLng0 = lng0;
    }
}

double lastLat,lastLng = 0;
void updateOdometer(){
    if (gps.location.isUpdated())
    {
        double lat = gps.location.lat();
        double lng = gps.location.lng();
        if (lastLat != 0 && lastLng != 0)
        {
            double distance = TinyGPSPlus::distanceBetween(
                lastLat, lastLng,
                lat, lng
            );
            if (distance > 2 && distance < 1000)
            {
                ode_km += distance / 1000.0;
            }
        }
        lastLat = lat;
        lastLng = lng;
    }
}

double lastLat1,lastLng1 = 0;
void updateOdometer1(){
    if (gps.location.isUpdated())
    {
        double lat1 = gps.location.lat();
        double lng1 = gps.location.lng();
        if (lastLat1 != 0 && lastLng1 != 0)
        {
            double distance1 = TinyGPSPlus::distanceBetween(
                lastLat1, lastLng1,
                lat1, lng1
            );
            if (distance1 > 2 && distance1 < 1000)
            {
                ode1_km += distance1 / 1000.0;
            }
        }
        lastLat1 = lat1;
        lastLng1 = lng1;
    }
}

double lastLat2,lastLng2 = 0;
void updateOdometer2(){
    if (gps.location.isUpdated())
    {
        double lat2 = gps.location.lat();
        double lng2 = gps.location.lng();
        if (lastLat2 != 0 && lastLng2 != 0)
        {
            double distance2 = TinyGPSPlus::distanceBetween(
                lastLat2, lastLng2,
                lat2, lng2
            );
            if (distance2 > 2 && distance2 < 1000)
            {
                ode2_km += distance2 / 1000.0;
            }
        }
        lastLat2 = lat2;
        lastLng2 = lng2;
    }
}

double lastLat3,lastLng3 = 0;
void updateOdometer3(){
    if (gps.location.isUpdated())
    {
        double lat3 = gps.location.lat();
        double lng3 = gps.location.lng();
        if (lastLat3 != 0 && lastLng3 != 0)
        {
            double distance3 = TinyGPSPlus::distanceBetween(
                lastLat3, lastLng3,
                lat3, lng3
            );
            if (distance3 > 2 && distance3 < 1000)
            {
                ode3_km += distance3 / 1000.0;
            }
        }
        lastLat3 = lat3;
        lastLng3 = lng3;
    }
}

void GetMaxSpeed(){
    if (gps.location.isUpdated()){
        float speed = gps.speed.kmph(); 
        if(maxSpeed < speed){
            maxSpeed=speed;
        }
    }
}

void GetAgvSpeed(){
    avgSpeed = (ode0_km / (time_ms / 1000.0)) * 3.6;
}

void setupOdometer(){
    File file = SD.open("/Odometer.json");
    if (!file) {
        Serial.println("Ошибка открытия файла");
        return;
    }else{
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, file);
        if (error) {
        Serial.println("Ошибка JSON");
        return;
        }
        ode_km = doc["ode"];
        ode1_km = doc["ode1"];
        ode2_km = doc["ode2"];
        ode3_km = doc["ode3"];
    }
}

void saveOdometer(){
    JsonDocument doc1;
    doc1["ode"] = ode_km;
    doc1["ode1"] = ode1_km;
    doc1["ode2"] = ode2_km;
    doc1["ode3"] = ode3_km;
    File file = SD.open("/Odometer.json", FILE_WRITE);
    if (!file) {
    Serial.println("Ошибка открытия файла");
    return;
    }else{
        serializeJsonPretty(doc1, file);
        file.close();
    }

}

void updateAllOdometer(){
    time_ms=millis()-time_start;
    updateOdometer();
    updateOdometer1();
    updateOdometer2();
    updateOdometer3();
    updateOdometer0();
    GetMaxSpeed();
    saveOdometer();
}