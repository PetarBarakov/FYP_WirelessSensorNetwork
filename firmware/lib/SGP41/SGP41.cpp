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

    SRAW_VOC = (uint16_t) ( (uint16_t) (rxBuffer[0] << 8) | rxBuffer[1]);
    SRAW_NOX = (uint16_t) ( (uint16_t) (rxBuffer[3] << 8) | rxBuffer[4]);
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

void SGP41::readSample(uint16_t& vocOut, uint16_t& noxOut, bool& vocjump, bool& noxjump)
{
    meaureRawSignal(vocOut, noxOut);

    const uint8_t averageBufferize = 10;

    static uint16_t averageVOCBuffer[averageBufferize] = {0};
    static uint16_t averageNOXBuffer[averageBufferize] = {0};

    uint32_t averageVOC = 0;
    uint32_t averageNOX = 0;

    for (uint8_t i = 0; i < averageBufferize - 1; i++)
    {
        averageVOCBuffer[i] = averageVOCBuffer[i + 1];
        averageNOXBuffer[i] = averageNOXBuffer[i + 1];

        averageVOC += averageVOCBuffer[i];
        averageNOX += averageNOXBuffer[i];
    }

    averageVOCBuffer[averageBufferize - 1] = vocOut;
    averageNOXBuffer[averageBufferize - 1] = noxOut;

    averageVOC += vocOut;
    averageNOX += noxOut;
    averageVOC /= averageBufferize;
    averageNOX /= averageBufferize;

    const int32_t VOCjumpTH = 1200; 
    const int32_t NOXjumpTH = 500; 

    // Serial.printf("VOC average: %d \t NOX average: %d\n", averageVOC, averageNOX);

    if(averageVOCBuffer[0] != 0)
    {
        if ( ((int32_t) (averageVOC - (uint32_t) vocOut ) > VOCjumpTH) || ( ( (int32_t) (averageVOC - (uint32_t)vocOut)) < -VOCjumpTH)) vocjump = true;
        else vocjump = false;

        if ( ((int32_t) (averageNOX - (uint32_t) noxOut ) > NOXjumpTH) || ( ( (int32_t) (averageNOX - (uint32_t)noxOut)) < -NOXjumpTH)) noxjump = true;
        else noxjump = false;
    }
}

// void SGP41::selftest()
// {    
//     uint8_t txBytes [2] = {0x28, 0x0E};

//     //Send the self test command
//     send
// }