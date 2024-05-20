#include "INA228.h"

INA228::INA228(uint8_t sensorAddress) : Sensor(sensorAddress)
{
}

void INA228::init(bool adcRange, uint16_t sampleAverage, uint16_t shuntResistanceVal)
{
    /* Write in configuration register
        Structure is:   RST|RSTACC|CONVDLY |TEMPCOMP|ADCRANGE|RESERVED
                        0  |0     |00000000|0       |0       |0000
    */
    
    /*ADC Range Configuration:
        0b     -->      +-163.84 mV
        1b     -->      +-40.96 mV
    */

    if(adcRange ==0) resolution = 163.84;
    else resolution = 40.96;

    uint8_t txBuffer [2];
    txBuffer[0] = 0x00 | adcRange << 4; 
    txBuffer[1] = 0x00;

    writeSensor1Byte(INA228_CONFIG_REG);
    writeSensorBytes(txBuffer, 2);

    /* Write in ADC Configuration register
        Structure is:   MODE    |VBUSTC |VSHCT  |VTCT   |AVG
                        1111    |101    |101    |101    |000
    */

   /*ADC Range Configuration:
        0b     -->      1 sample
        1b     -->      4 samples
        2b     -->      16 samples
        3b     -->      64 samples
        4b     -->      128 samples
        5b     -->      256 samples
        6b     -->      512 samples
        7b     -->      1024 samples
    */

   uint8_t sampleAverageBits = 0;

    switch (sampleAverage)
    {
    case 1:
        sampleAverageBits = 0b000;
        break;

    case 4:
        sampleAverageBits = 0b001;
        break;

    case 16:
        sampleAverageBits = 0b010;
        break;

    case 64:
        sampleAverageBits = 0b011;
        break;

    case 128:
        sampleAverageBits = 0b100;
        break;

    case 256:
        sampleAverageBits = 0b101;
        break;

    case 512:
        sampleAverageBits = 0b110;
        break;

    case 1024:
        sampleAverageBits = 0b111;
        break;
    
    default:
        sampleAverageBits = 0b000;
        break;
    }
    
    txBuffer[0] = 0x68 | sampleAverageBits; //1111 1011 0110 1000 | sampleAverageBits
    txBuffer[1] = 0xFB;

    writeSensor1Byte(INA228_ADC_CONFIG_REG);
    writeSensorBytes(txBuffer, 2);

    //Shunt Calibration

    txBuffer[0] = shuntResistanceVal;
    txBuffer[1] = shuntResistanceVal >> 8;

    writeSensor1Byte(INA228_SHUNT_CALIBRATION_REG);
    writeSensorBytes(txBuffer, 2);
}

void INA228::readShuntVoltage(double &shuntVoltage)
{
    writeSensor1Byte(INA228_VSHUNT_REG);
    uint8_t rxBuffer [3];
    readSensorBytes(rxBuffer, 3);

    int32_t ShuntRawVoltage = rxBuffer[2] << 16 | rxBuffer[1] << 8 | rxBuffer[0];
    ShuntRawVoltage = ShuntRawVoltage >> 4;
    if (ShuntRawVoltage & 0x80000) ShuntRawVoltage = ShuntRawVoltage | 0xFFF00000;

    shuntVoltage = ShuntRawVoltage * resolution / 1000.0; //result in milivolts
}

void INA228::readBusVoltage(double &busVoltage)
{
    writeSensor1Byte(INA228_VBUS_REG);
    uint8_t rxBuffer [3];
    readSensorBytes(rxBuffer, 3);

    int32_t BusRawVoltage = rxBuffer[2] << 16 | rxBuffer[1] << 8 | rxBuffer[0];
    BusRawVoltage = BusRawVoltage >> 4;

    if (BusRawVoltage & 0x80000) BusRawVoltage = BusRawVoltage | 0xFFF00000;

    busVoltage = BusRawVoltage * 195.3125 / 1000.0; //result in milivolts
}

void INA228::readPower(uint32_t &power)
{
    writeSensor1Byte(INA228_POWER_REG);
    uint8_t rxBuffer [3];

    uint32_t powerRaw = rxBuffer[2] << 16 | rxBuffer[1] << 8 | rxBuffer[0];
    power = powerRaw;
}

void INA228::readCurrent(int32_t &current)
{
    writeSensor1Byte(INA228_CURRENT_REG);
    uint8_t rxBuffer [3];
    readSensorBytes(rxBuffer, 3);

    int32_t CurrentRaw = rxBuffer[2] << 16 | rxBuffer[1] << 8 | rxBuffer[0];
    CurrentRaw = CurrentRaw >> 4;
    if (CurrentRaw & 0x80000) CurrentRaw = CurrentRaw | 0xFFF00000;
    

    current = CurrentRaw * 195.3125 / 1000.0; //result in milivolts
}

void INA228::reset()
{
    uint8_t txBuffer [2];
    txBuffer[0] = 0x00; 
    txBuffer[1] = 0x80;

    writeSensor1Byte(INA228_CONFIG_REG);
    writeSensorBytes(txBuffer, 2);
}

