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

double lat,lng=0;
double lastLat,lastLng = 0;

double ode0_km;
void updateOdometer0(){
    double distance0 = TinyGPSPlus::distanceBetween(
       lastLat, lastLng,
        lat, lng
    );
   if (distance0 > 0 && distance0 < 1000)
    {
        ode0_km += distance0 / 1000.0;
    }
}

void updateOdometer(){
    double distance = TinyGPSPlus::distanceBetween(
        lastLat, lastLng,
        lat, lng
    );
    if (distance > 0 && distance < 1000)
    {
        ode_km += distance / 1000.0;
    }
}

void updateOdometer1(){
    double distance1 = TinyGPSPlus::distanceBetween(
        lastLat, lastLng,
       lat, lng
    );
    if (distance1 > 0 && distance1 < 1000)
    {
        ode1_km += distance1 / 1000.0;
    }
}

void updateOdometer2(){
    double distance2 = TinyGPSPlus::distanceBetween(
        lastLat, lastLng,
        lat, lng
    );
    if (distance2 > 0 && distance2 < 1000)
    {
        ode2_km += distance2 / 1000.0;
    }
}

void updateOdometer3(){
    double distance3 = TinyGPSPlus::distanceBetween(
        lastLat, lastLng,
        lat, lng
    );
    if (distance3 > 0 && distance3 < 1000)
    {
        ode3_km += distance3 / 1000.0;
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

void setupOdometer() {
    // Проверяем, существует ли файл
    if (!SD.exists("/Odometer.json")) {
        Serial.println("Файл Odometer.json не найден, создаем новый...");

        File newFile = SD.open("/Odometer.json", FILE_WRITE);
        if (newFile) {
            // Создаём пустой JSON
            newFile.println("{\"ode\":0,\"ode1\":0,\"ode2\":0,\"ode3\":0}");
            newFile.close();
            Serial.println("Файл создан!");
        } else {
            Serial.println("Не удалось создать файл Odometer.json");
            return;
        }
    }

    // Открываем файл для чтения
    File file = SD.open("/Odometer.json");
    if (!file) {
        Serial.println("Ошибка открытия файла после создания");
        return;
    }

    // Читаем данные из JSON
    StaticJsonDocument<256> doc;  // Можно заменить на JsonDocument<256> при необходимости
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        Serial.println("Ошибка парсинга JSON");
        return;
    }

    // ✅ Сериализация документа перед выводом в Serial
    serializeJsonPretty(doc, Serial);
    Serial.println(); // добавляем перевод строки после JSON

    ode_km  = doc["ode"].as<double>();
    ode1_km = doc["ode1"].as<double>();
    ode2_km = doc["ode2"].as<double>();
    ode3_km = doc["ode3"].as<double>();

    Serial.println("Odometer загружен!");
}

void saveOdometer() {
    File file = SD.open("/Odometer.json", FILE_WRITE);
    if (!file) {
        Serial.println("Ошибка открытия файла для записи");
        return;
    }

    StaticJsonDocument<256> doc;
    doc["ode"]  = ode_km;
    doc["ode1"] = ode1_km;
    doc["ode2"] = ode2_km;
    doc["ode3"] = ode3_km;

    // Записываем в файл красиво
    serializeJsonPretty(doc, file);
    file.close();

    // ✅ Также можно вывести в Serial для отладки
    serializeJsonPretty(doc, Serial);
    Serial.println();
    Serial.println("Odometer сохранен!");
}

void updateAllOdometer(){
    time_ms=millis()-time_start;
    if (gps.location.isUpdated()){
        if (lastLat != 0 && lastLng != 0){
            lat = gps.location.lat();
            lng = gps.location.lng();
            updateOdometer();
            updateOdometer1();
            updateOdometer2();
            updateOdometer3();
            updateOdometer0();
            GetMaxSpeed();
            saveOdometer();
            lastLat = lat;
            lastLng = lng;
        }
    }
}