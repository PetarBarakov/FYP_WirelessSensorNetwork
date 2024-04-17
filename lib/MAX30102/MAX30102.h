#ifndef MAX30102_H
#define MAX30102_H

#include <Arduino.h>
#include <Wire.h>
#include "SensorClass.h"

// Main Configuration Register Addresses
#define FIFO_CONFIG 0x08
#define MODE_CONFIG 0x09
#define SPO2_CONFIG 0x0A
// TODO: Add LED configs and MultiLedControl



class MAX30102 : public Sensor
{
    public:
    MAX30102(uint8_t sensorAddress);

    void SPO2read();
    void HRread();

    private:
    void readFIFO();

};

#endif