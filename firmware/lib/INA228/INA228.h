#ifndef INA228_H
#define INA228_H

#include <Arduino.h>
#include <Wire.h>
#include "SensorClass.h"

#define INA228_CONFIG_REG 0x0
#define INA228_ADC_CONFIG_REG 0x1
#define INA228_SHUNT_CALIBRATION_REG 0x2
// #define INA228_SHUNT_TEMP_COEFFICIENT_REG 0x3
#define INA228_VSHUNT_REG 0x4
#define INA228_VBUS_REG 0x5
// #define INA228_DIE_TEMP_REG 0x6
#define INA228_CURRENT_REG 0x7
#define INA228_POWER_REG 0x8
// #define INA228_ENERGY_REG 0x9
// #define INA228_CHARGE_REG 0xA

// #define INA228_DIAG_ALRT_REG 0xB
// #define INA228_SOVL_REG 0xC
// #define INA228_SUVL_REG 0xD
// #define INA228_BOVL_REG 0xE
// #define INA228_BUVL_REG 0xF
// #define INA228_TEMP_LIMIT_REG 0x10
// #define INA228_POWER_LIMIT_REG 0x11

class INA228 : public Sensor
{
    public:
    INA228(uint8_t sensorAddress);

    void init(bool adcRange, uint16_t sampleAverage, uint16_t shuntResistanceVal);
    void readShuntVoltage(double &shuntVoltage);
    void readBusVoltage(double &busVoltage);
    void readPower(uint32_t &power);
    void readCurrent(double &current);

    private:
    void reset();

    double resolution;
};

#endif