#ifndef SHT40_H
#define SHT40_H

#include <Arduino.h>
#include <Wire.h>
#include "SensorClass.h"

#define COMMAND_TRH_HIGH_PREC 0xFD
#define COMMAND_TRH_MED_PREC 0xF6
#define COMMAND_TRH_LOW_PREC 0xE0

#define COMMAND_TRH_RESET 0x94

class SHT40 : public Sensor
{
    public:
    SHT40(uint8_t sensorAddress);

    void readTempHumid(double& tempReadOut, double& RHReadOut, uint8_t precission = 1);
    void softReset();
};

#endif