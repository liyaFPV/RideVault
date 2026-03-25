
#pragma once
#include "Arduino.h"
#include "config.h"


struct Note {
    int freq;
    int dur;
};

extern Note gps_found[3];
extern Note gps_lost[3];

extern Note wifi_on[3];
extern Note wifi_off[3];

extern Note ble_connected[3];
extern Note ble_disconnected[3];

extern Note system_start[3];
extern Note system_shutdown[3];

void test_mid();
void play_mid(Note *melody);
