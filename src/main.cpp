#include <Arduino.h>
#include "SPI.h"
#include "sd.h"
#include "config.h"
#include <TinyGPS++.h>
#include "ble.h"
#include "Odometer.h"
#include "mid.h"
#include "gpsloger.h"

// GPS
TinyGPSPlus gps;

HardwareSerial gpsSerial(1);

bool gpsFLAG = false;

void setup() {
    Serial.begin(115200);
    pinMode(BUZER_PIN,OUTPUT);
    ledcSetup(0, 2000, 8);
    ledcAttachPin(BUZER_PIN, 0);
    sd_init();
    sd_info();
    ble_begin();
    // ---- GPS ----
    Serial.println("Запуск GPS...");

    gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX, GPS_TX);

    Serial.println("GPS готов");
    setupOdometer();
    play_mid(system_start);
}

void loop() {
  ble_loop();
    while (gpsSerial.available()) {
        char c = gpsSerial.read();
        gps.encode(c);
    } 
    if (gpsFLAG) {
        updateAllOdometer();
        update_csv();
        Serial.println();
        Serial.println("------ GPS ------");

        Serial.print("Lat: ");
        Serial.println(gps.location.lat(), 6);

        Serial.print("Lng: ");
        Serial.println(gps.location.lng(), 6);

        Serial.print("Satellites: ");
        Serial.println(gps.satellites.value());

        if (gps.date.isValid()) {
            Serial.printf("Date: %02d/%02d/%04d\n",
                          gps.date.day(),
                          gps.date.month(),
                          gps.date.year());
        }
        if (gps.time.isValid()) {
            Serial.printf("Time: %02d:%02d:%02d\n",
                          gps.time.hour()+(time_zone),
                          gps.time.minute(),
                          gps.time.second());
        }
        Serial.printf("speed %.2f\n",gps.speed.kmph());
        Serial.print("ode ");  Serial.println(ode_km, 2);
        Serial.print("ode1 "); Serial.println(ode1_km, 2);
        Serial.print("ode2 "); Serial.println(ode2_km, 2);
        Serial.print("ode3 "); Serial.println(ode3_km, 2);

        Serial.println("-----------------");
    }
    
    if(gps.satellites.value()>0 && gps.date.isValid() && !gpsFLAG){
        gpsFLAG = true;
        play_mid(gps_found);
        setup_csv();
    }
    if(gps.satellites.value()<=0 and gpsFLAG){
        gpsFLAG = false;
        play_mid(gps_lost);
    }
    delay(1000);
}