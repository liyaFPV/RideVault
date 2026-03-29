#include "gpsloger.h"
#include <TinyGPS++.h>
#include <SD.h>
#include "config.h"
#include "Odometer.h"
extern TinyGPSPlus gps;

String buffer,csv_fileNAME,timeStr;

void setup_csv(){
    csv_fileNAME = String(gps.date.year()) + ":" + String(gps.date.month()) + ":" + String(gps.date.day()) + ".csv";
    File csvFile = SD.open(csv_fileNAME, FILE_APPEND);
    if(!csvFile){
        Serial.println("Не удалось открыть файл для записи");
        return;
    }
    csvFile.println("0x00");
    csvFile.close();
    return;
}

void update_csv(){
    timeStr=String(gps.time.hour()+(time_zone))+":"+String(gps.time.minute())+":"+String(gps.time.second());
    buffer=String(gps.location.lat())+","+String(gps.location.lng())+","+String(timeStr)+","+String(gps.speed.kmph())+","+String(avgSpeed)+","+String(maxSpeed);
    File csvFile = SD.open(csv_fileNAME, FILE_APPEND);
    if(!csvFile){
        Serial.println("Не удалось открыть файл для записи");
        return;
    }
    csvFile.println(buffer);
    csvFile.close();

    return;
}