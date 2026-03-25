#include "ble.h"

bool deviceConnected = false;
bool bledeviceConnected = false;
#define SERVICE_UUID         "12345678-1234-1234-1234-123456789abc"
#define SPEED_CHAR_UUID      "11111111-1111-1111-1111-111111111111"
#define AVG_SPEED_CHAR_UUID  "22222222-2222-2222-2222-222222222222"
#define MAX_SPEED_CHAR_UUID  "33333333-3333-3333-3333-333333333333"
#define ODE1_CHAR_UUID       "44444444-4444-4444-4444-444444444444"
#define ODE2_CHAR_UUID       "55555555-5555-5555-5555-555555555555"
#define ODE3_CHAR_UUID       "66666666-6666-6666-6666-666666666666"

BLECharacteristic* speed;
BLECharacteristic* avg_speed;
BLECharacteristic* max_speed;
BLECharacteristic* ode1;
BLECharacteristic* ode2;
BLECharacteristic* ode3;



class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) { deviceConnected = true; }
  void onDisconnect(BLEServer* pServer) { deviceConnected = false; }
};

void ble_begin() {
  BLEDevice::init("ESP32_CustomBLE");
  
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  BLEService *pService = pServer->createService(SERVICE_UUID);

  speed = pService->createCharacteristic(
                        SPEED_CHAR_UUID,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_NOTIFY
                      );
  speed->addDescriptor(new BLE2902());

  avg_speed = pService->createCharacteristic(
                        AVG_SPEED_CHAR_UUID,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_NOTIFY
                      );
  avg_speed->addDescriptor(new BLE2902());

  max_speed = pService->createCharacteristic(
                        MAX_SPEED_CHAR_UUID,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_NOTIFY
                      );
  max_speed->addDescriptor(new BLE2902());

  ode1 = pService->createCharacteristic(
                        ODE1_CHAR_UUID,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_NOTIFY
                      );
  ode1->addDescriptor(new BLE2902());

  ode2 = pService->createCharacteristic(
                        ODE2_CHAR_UUID,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_NOTIFY
                      );
  ode2->addDescriptor(new BLE2902());

  ode3 = pService->createCharacteristic(
                        ODE3_CHAR_UUID,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_NOTIFY
                      );
  ode3->addDescriptor(new BLE2902());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
}

void ble_loop() {

  if (deviceConnected) {

    double spd = gps.speed.kmph();

    speed->setValue(String(spd).c_str());
    speed->notify();

    avg_speed->setValue(String(avgSpeed).c_str());
    avg_speed->notify();

    max_speed->setValue(String(maxSpeed).c_str());
    max_speed->notify();

    ode1->setValue(String(ode1_km).c_str());
    ode1->notify();

    ode2->setValue(String(ode2_km).c_str());
    ode2->notify();

    ode3->setValue(String(ode3_km).c_str());
    ode3->notify();
  }

  if (deviceConnected && !bledeviceConnected) {
      bledeviceConnected = true;
      play_mid(ble_connected);
  }

  if (!deviceConnected && bledeviceConnected) {
      bledeviceConnected = false;
      play_mid(ble_disconnected);
  }

}