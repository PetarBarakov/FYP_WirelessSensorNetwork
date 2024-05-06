#include "SensorClass.h"

// void Sensor::setAddress(uint8_t sensorAddress)
// {
//     Sensor::address = sensorAddress;
// }

Sensor::Sensor(uint8_t sensorAddress)
{
    address = sensorAddress;
}

void Sensor::readSensorBytes(uint8_t* rxBytes, uint8_t numBytes)
{
    Wire.requestFrom (address, numBytes);

    for (uint8_t i = 0; Wire.available(); i++)
    {
        *(rxBytes + i) = Wire.read();
    }

    //returns an array of numBytes for the received bytes 
}

void Sensor::writeSensorBytes(uint8_t* txBytes, uint8_t numBytes)
{
    Wire.beginTransmission(address);

    for(uint8_t i = 0; i < numBytes; i++)
    {
        Wire.write(*(txBytes + i));
    }

    Wire.endTransmission();
}

void Sensor::writeToReg(uint8_t regAddr, uint8_t byteValue)
{
    Wire.beginTransmission(address);

    Wire.write(regAddr);
    Wire.write(byteValue);

    Wire.endTransmission();
}

void Sensor::writeSensor1Byte(uint8_t txByte)
{
    Wire.beginTransmission(address);

    Wire.write(txByte);

    Wire.endTransmission();
}