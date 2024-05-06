#include "SGP41.h"

SGP41::SGP41(uint8_t sensorAddress) : Sensor(sensorAddress) 
{
}

void SGP41::executeConditioning(uint16_t& SRAW_VOC_INTIAL)
{
    uint8_t txBytes[8] = {0x26, 0x12, 0x80, 0x00, 0xA2, 0x66, 0x66, 0x93};
    
    //Send the conditioning command
    writeSensorBytes(txBytes, 8);

    delay(9000); //delay 9 seconds. The maximum allowed delay is 10s

    uint8_t rxBuffer[3];
    readSensorBytes(rxBuffer, 3);

    SRAW_VOC_INTIAL = (rxBuffer[1] << 8) | rxBuffer[0];
}

void SGP41::meaureRawSignal(uint16_t& SRAW_VOC, uint16_t& SRAW_NOX)
{
    //Measure Raw Data without tempearture and humidity compensation
    uint8_t txBytes [8] = {0x26, 0x19, 0x80, 0x00, 0xA2, 0x66, 0x66, 0x93};
    writeSensorBytes(txBytes, 8);

    delay(50);

    uint8_t rxBuffer[6];

    readSensorBytes(rxBuffer, 6);

    SRAW_VOC = (rxBuffer[1] << 8) | rxBuffer[0];
    SRAW_NOX = (rxBuffer[4] << 8) | rxBuffer[3];
}

void SGP41::turnHeaterOff()
{
    uint8_t txBytes[2] = {0x36, 0x15};

    //Send turn heater off commands
    writeSensorBytes(txBytes, 2);
}

void SGP41::reset()
{
    uint8_t txBytes[2] = {0x00, 0x06};

    //Send the reset commands
    writeSensorBytes(txBytes, 2);
}

void SGP41::readSample(double& vocOut, double& noxOut)
{
    uint16_t SRAW_VOC, SRAW_NOX;

    meaureRawSignal(SRAW_VOC, SRAW_NOX);

    vocOut = SRAW_VOC * 1.0;
    noxOut = SRAW_NOX * 1.0;
}

// void SGP41::selftest()
// {
//     uint8_t txBytes [2] = {0x28, 0x0E};

//     //Send the self test command
//     send
// }