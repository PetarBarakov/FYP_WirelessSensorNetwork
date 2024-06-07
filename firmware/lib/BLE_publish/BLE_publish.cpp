#include "BLE_publish.h"

void MyServerCallbacks::onConnect(BLEServer* pServer) {
    deviceConnected = true;
}

void MyServerCallbacks::onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
}


publisherBLE::publisherBLE(char* setupName, char* charUUID, char* serviceUUID)
{
    BLEname = setupName;
    char_UUID = charUUID;
    service_UUID = serviceUUID;
}
   
void publisherBLE::BLEinit()
{
    // Create the BLE Device
    BLEDevice::init(BLEname);

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    Callbacks_handle = new MyServerCallbacks();
    pServer->setCallbacks(Callbacks_handle);

    // Create the BLE Service
    BLEService *pService = pServer->createService(service_UUID);

    // Create a BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
                        char_UUID,
                        BLECharacteristic::PROPERTY_NOTIFY
    );


    // Create a BLE Descriptor
    pCharacteristic->addDescriptor(new BLE2902());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    BLEDevice::startAdvertising();
}

void publisherBLE::BLEsendValue(char* transmitValue)
{
    if (Callbacks_handle->deviceConnected) {
        pCharacteristic->setValue(transmitValue);
        // Serial.printf("The transmitted values is: %s.\n", transmitValue);
        pCharacteristic->notify();
    }
    // disconnecting
    if (!Callbacks_handle->deviceConnected && Callbacks_handle->oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        Callbacks_handle->oldDeviceConnected = Callbacks_handle->deviceConnected;
    }
    // connecting
    if (Callbacks_handle->deviceConnected && !Callbacks_handle->oldDeviceConnected) {
        // do stuff here on connecting
        Callbacks_handle->oldDeviceConnected = Callbacks_handle->deviceConnected;
    }
}
