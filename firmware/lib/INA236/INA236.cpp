#include "INA236.h"

INA236::INA236(uint8_t sensorAddress) : Sensor(sensorAddress)
{
}

void INA236::init(bool adcRange, uint16_t sampleAverage, uint16_t shuntResistanceVal)
{
    configure(adcRange, sampleAverage);
    calibrate(shuntResistanceVal);

    if(adcRange == 0) resolution = 2.5 / 1000;
    if(adcRange == 1) resolution = 0.625 / 1000;
}

void INA236::readShuntVoltage(double &shuntVoltage)
{
    writeSensor1Byte(INA236_SHUNTVOLTAGE_REG);
    uint8_t rxShuntRaw [2];
    readSensorBytes(rxShuntRaw, 2);

    int16_t ShuntRawVoltage= (rxShuntRaw[0] << 8) | rxShuntRaw[1];
    
    shuntVoltage = double (ShuntRawVoltage * resolution); //result in milivolts
}

void INA236::readBusVoltage(double &busVoltage)
{
    writeSensor1Byte(INA236_BUSVOLTAGE_REG);
    uint8_t rxBusRaw [2];
    readSensorBytes(rxBusRaw, 2);

    busVoltage = (double) (((uint32_t)(rxBusRaw[0] << 8) | rxBusRaw[1]) * 1.6e-3);
}

void INA236::readPower(uint32_t &power)
{
    writeSensor1Byte(INA236_POWER_REG);
    uint8_t rxPowerRaw [2];
    readSensorBytes(rxPowerRaw, 2);

    power = (rxPowerRaw[0] << 8) | rxPowerRaw[1];
}

void INA236::readCurrent(double &current)
{
    writeSensor1Byte(INA236_CURRENT_REG);
    uint8_t rxCurrentRaw [2];
    readSensorBytes(rxCurrentRaw, 2);

    current = (double) ( (int32_t) ( (rxCurrentRaw[0] << 8) | rxCurrentRaw[1]) * resolution );
    
}

void INA236::configure(bool adcRange, uint16_t sampleAverage)
{
    /*ADC Range Configuration:
        0b     -->      +-81.92 mV
        1b     -->      +-20.48 mV
    */

    /*Sample Average
        000b   -->      1 sample
        001b   -->      4 sample
        010b   -->      16 sample
        011b   -->      64 sample
        100b   -->      128 sample
        101b   -->      256 sample
        110b   -->      512 sample
        111b   -->      1024 sample
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

    uint8_t bufferBytes [2];
    
    // 0  |10|0       |000|1 \00 |100  |111     --> 0100 0001 | 0010 0111
    // RST|10|adcrange|avg|vbusct|vshct|mode

    bufferBytes [0] = 0x27;
    bufferBytes [1] = (0x41 | (adcRange << 5) ) | sampleAverageBits << 1;

    writeSensor1Byte(INA236_CONFIG_REG);
    writeSensorBytes(bufferBytes, 2);
}

void INA236::calibrate(uint16_t shuntResistanceVal)
{
    uint8_t bufferBytes [2];

    bufferBytes [1] = (shuntResistanceVal >> 8);
    bufferBytes [0] = shuntResistanceVal;

    writeSensor1Byte(INA236_CALIBRATION_REG);
    writeSensorBytes(bufferBytes, 2);
}

void INA236::reset()
{
    uint8_t bufferBytes [2];

    bufferBytes [1] = 0xC0; //0b1100 0000
    bufferBytes [0] = 0;

    writeSensor1Byte(INA236_CONFIG_REG);
    writeSensorBytes(bufferBytes, 2);
}