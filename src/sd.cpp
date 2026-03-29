#include "sd.h"
uint8_t cardType;

bool sd_status = false;

bool get_sdStatus(){
    return sd_status;
}

bool sd_init()
{
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);

    if (!SD.begin(SD_CS, SPI, 2000000))
    {
        Serial.println("SD init failed");
        return false;
    }

    Serial.println("SD init OK");
    sd_status=true;
    return true;
}

void sd_info()
{
    cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");

    if (cardType == CARD_MMC) Serial.println("MMC");
    else if (cardType == CARD_SD) Serial.println("SDSC");
    else if (cardType == CARD_SDHC) Serial.println("SDHC");
    else Serial.println("UNKNOWN");

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    uint64_t totalBytes = SD.totalBytes() / (1024 * 1024);
    uint64_t usedBytes = SD.usedBytes() / (1024 * 1024);

    Serial.printf("Total space: %lluMB\n", totalBytes);
    Serial.printf("Used space: %lluMB\n", usedBytes);
}
uint64_t sd_get_size()
{
    return SD.cardSize() / (1024 * 1024);
}