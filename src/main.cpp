#include <Arduino.h>
#include "SPI.h"
#include "SD.h"
#include "config.h"
#include <TinyGPS++.h>
#include "ble.h"
#include "Odometer.h"
#include "mid.h"

// SPI для SD
SPIClass spi_ext(FSPI);

// GPS
TinyGPSPlus gps;

HardwareSerial gpsSerial(1);

bool gpsFLAG = false;

void setup() {
    Serial.begin(115200);

    // ---- SD CARD ----
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);

    pinMode(SD_SCK, OUTPUT);
    pinMode(SD_MOSI, OUTPUT);
    pinMode(SD_MISO, INPUT_PULLUP);
    pinMode(BUZER_PIN,OUTPUT);

    Serial.println("Инициализация SPI...");
    spi_ext.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

    if (!SD.begin(SD_CS, spi_ext, 1000000)) {
        Serial.println("Ошибка SD карты");
    } else {
        Serial.println("SD карта готова");
        Serial.printf("Размер: %llu MB\n", SD.cardSize() / (1024 * 1024));
    }
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
  updateAllOdometer();
    if (gps.location.isUpdated()) {

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
        Serial.printf("ode %02d\n",ode_km);
        Serial.printf("ode1 %02d\n",ode1_km);
        Serial.printf("ode2 %02d\n",ode2_km);
        Serial.printf("ode3 %02d\n",ode3_km);

        Serial.println("-----------------");
    }
    
    if(gps.satellites.value()>0 and !gpsFLAG){
        gpsFLAG = true;
        play_mid(gps_found);
    }

    if(gps.satellites.value()<=0 and gpsFLAG){
        gpsFLAG = false;
        play_mid(gps_lost);
    }
    delay(1000);
}