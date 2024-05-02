#include <Arduino.h>
#include <Wire.h>
#include "SensorClass.h"

// Note each register is 2 Bytes
#define INA236_CONFIG_REG 0x0
#define INA236_SHUNTVOLTAGE_REG 0x1
#define INA236_BUSVOLTAGE_REG 0x2
#define INA236_POWER_REG 0x3
#define INA236_CURRENT_REG 0x4
#define INA236_CALIBRATION_REG 0x5
// #define INA236_EN_REG 0x6
// #define INA236_ALERTLIMIT_REG 0x1

class INA236 : public Sensor
{
    public:
    INA236(uint8_t sensorAddress);
    void init(bool adcRange, uint16_t sampleAverage, uint16_t shuntResistanceVal);

    void readShuntVoltage(double &shuntVoltage);
    void readBusVoltage(uint16_t &busVoltage);
    void readPower(uint16_t &power);
    void readCurrent(double &current);
    
    private:
    void configure(bool adcRange, uint16_t sampleAverage);
    void calibrate(uint16_t shuntResistanceVal);
    void reset();

    double resolution;

};