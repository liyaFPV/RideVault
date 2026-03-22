#include "ble.h"

bool deviceConnected = false;

#define SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"
#define TEMP_CHAR_UUID      "11111111-1111-1111-1111-111111111111"
#define HUM_CHAR_UUID       "22222222-2222-2222-2222-222222222222"

BLECharacteristic* tempCharacteristic;
BLECharacteristic* humCharacteristic;

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) { deviceConnected = true; }
  void onDisconnect(BLEServer* pServer) { deviceConnected = false; }
};

void ble_begin() {
  BLEDevice::init("ESP32_CustomBLE");
  
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  BLEService *pService = pServer->createService(SERVICE_UUID);

  tempCharacteristic = pService->createCharacteristic(
                        TEMP_CHAR_UUID,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_NOTIFY
                      );
  tempCharacteristic->addDescriptor(new BLE2902());

  humCharacteristic = pService->createCharacteristic(
                        HUM_CHAR_UUID,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_NOTIFY
                      );
  humCharacteristic->addDescriptor(new BLE2902());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
}

void ble_loop() {
  if (deviceConnected) {
    float temp = 25.5;
    float hum = 60.2;

    tempCharacteristic->setValue(temp);
    tempCharacteristic->notify();

    humCharacteristic->setValue(hum);
    humCharacteristic->notify();
  }
}