#include "INA236.h"

INA236::INA236(uint8_t sensorAddress) : Sensor(sensorAddress)
{
}

void INA236::init(bool adcRange, uint8_t sampleAverage, uint16_t shuntResistanceVal)
{

}

void INA236::readShuntVoltage(double &shuntVoltage)
{

}

void INA236::readBusVoltage(uint16_t &busVoltage)
{

}

void INA236::readPower(uint16_t &power)
{

}

void INA236::readCurrent(double &current)
{

}

void INA236::configure(bool adcRange, uint8_t sampleAverage)
{

}

void INA236::calibrate(uint16_t shuntResistanceVal)
{
    
}