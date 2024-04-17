#ifndef SENSORCLASS_H
#define SENSORCLASS_H

#include <Arduino.h>
#include <Wire.h>


class Sensor{

    public:
    Sensor(uint8_t senssdforAddress);

    // void setAddress(uint8_t sensorAddress);

    protected:

    void readSensorBytes(uint8_t* rxBytes, uint8_t numBytes);
    void writeSensorBytes(uint8_t* txBytes, uint8_t numBytes);
    void writeSensor1Byte(uint8_t txByte);
    uint8_t address;
    
};

#endif