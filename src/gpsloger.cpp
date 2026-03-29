#include "gpsloger.h"
#include <TinyGPS++.h>
#include <SD.h>
#include "config.h"
#include "Odometer.h"
extern TinyGPSPlus gps;
extern bool gpsFLAG;

String buffer,csv_fileNAME,timeStr;

void setup_csv(){

    csv_fileNAME = "/" + String(gps.date.year()) + "-" +(gps.date.month()<10?"0":"") + String(gps.date.month()) + "-" +(gps.date.day()<10?"0":"") + String(gps.date.day()) +".csv";
    Serial.print("csv_fileNAME: ");
    Serial.println(csv_fileNAME);
    
    if(!SD.exists(csv_fileNAME)){
        Serial.println("Создаю новый CSV файл");

        File csvFile = SD.open(csv_fileNAME, FILE_WRITE);
        if(!csvFile){
            Serial.println("Ошибка создания файла");
            return;
        }

        csvFile.println("0x00"); // header CSV
        csvFile.close();
    }
}

void update_csv(){

    if(gpsFLAG){

        timeStr = String(gps.time.hour() + time_zone) + ":" +
                  String(gps.time.minute()) + ":" +
                  String(gps.time.second());

        buffer = String(gps.location.lat(),6) + "," +
                 String(gps.location.lng(),6) + "," +
                 timeStr + "," +
                 String(gps.speed.kmph()) + "," +
                 String(avgSpeed) + "," +
                 String(maxSpeed);

        File csvFile = SD.open(csv_fileNAME, FILE_APPEND);

        if(!csvFile){
            Serial.println("Не удалось открыть файл");
            return;
        }

        csvFile.println(buffer);
        csvFile.close();
    }
}