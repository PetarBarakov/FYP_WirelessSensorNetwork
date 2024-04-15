#ifndef BLE_PUBLISH_H
#define BLE_PUBLISH_H

#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Define UUIDs for the Service and for the Characteristic
// Both should vary from device to device (node to node)
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyServerCallbacks: public BLEServerCallbacks {
    public:
    bool deviceConnected;
    bool oldDeviceConnected;


    private:
    void onConnect(BLEServer* pServer);
    void onDisconnect(BLEServer* pServer);
};

class publisherBLE{

    private:

    BLEServer* pServer;                 //Handle for the server/device
    BLECharacteristic* pCharacteristic; //Handle for the characteristic


    char* BLEname;
    MyServerCallbacks* Callbacks_handle;
    
    public:

    publisherBLE(char* setupName);
    void BLEinit();
    void BLEsendValue(int transmitValue);

};


#endif