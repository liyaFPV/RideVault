#include "mid.h"

// GPS найден — восходящая
Note gps_found[3] = {
    {1047, 150}, // C6
    {1319, 150}, // E6
    {1568, 200}  // G6
};

// GPS потерян — нисходящая
Note gps_lost[3] = {
    {1568, 150}, // G6
    {1319, 150}, // E6
    {1047, 200}  // C6
};

// WiFi включился — чередующиеся вверх
Note wifi_on[3] = {
    {988, 150},  // B5
    {1175, 150}, // D6
    {1319, 200}  // E6
};

// WiFi выключился — чередующиеся вниз
Note wifi_off[3] = {
    {1319, 150}, // E6
    {1175, 150}, // D6
    {988, 200}   // B5
};

// BLE подключился — восходящий триплет
Note ble_connected[3] = {
    {880, 150},  // A5
    {1047, 150}, // C6
    {1175, 200}  // D6
};

// BLE отключился — нисходящий триплет
Note ble_disconnected[3] = {
    {1175, 150}, // D6
    {1047, 150}, // C6
    {880, 200}   // A5
};

// Система включилась — короткий восходящий
Note system_start[3] = {
    {523, 150},  // C5
    {659, 150},  // E5
    {784, 200}   // G5
};

// Система выключилась — короткий нисходящий
Note system_shutdown[3] = {
    {784, 150},  // G5
    {659, 150},  // E5
    {523, 200}   // C5
};

void play_mid(Note *melody) {
  for (int i = 0; i < 3; i++) {
    tone(BUZER_PIN, melody[i].freq, melody[i].dur);
    delay(melody[i].dur);
  }
}

void test_mid(){
    Serial.println("gps_found");
    play_mid(gps_found);
    delay(500);

    Serial.println("gps_lost");
    play_mid(gps_lost);
    delay(500);

    Serial.println("wifi_on");
    play_mid(wifi_on);
    delay(500);

    Serial.println("wifi_off");
    play_mid(wifi_off);
    delay(500);

    Serial.println("ble_connected");
    play_mid(ble_connected);
    delay(500);

    Serial.println("ble_disconnected");
    play_mid(ble_disconnected);
    delay(500);

    Serial.println("system_start");
    play_mid(system_start);
    delay(500);

    Serial.println("system_shutdown");
    play_mid(system_shutdown);
    delay(500);
}