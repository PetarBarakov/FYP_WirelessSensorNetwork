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
    char* UUID;
    MyServerCallbacks* Callbacks_handle;
    
    public:

    publisherBLE(char* setupName, char* charUUID);
    void BLEinit();
    void BLEsendValue(char* transmitValue);

};


#endif