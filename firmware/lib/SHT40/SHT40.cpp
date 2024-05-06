#include "SHT40.h"

SHT40::SHT40(uint8_t sensorAddress) : Sensor(sensorAddress)
{

}

void SHT40::readTempHumid(double& tempReadOut, double& RHReadOut, uint8_t precission)
{
    /*
    There are three options for the precission:
        precission = 0  --> Low precission, low reapeatability, low current of 0.4muA
        precission = 1  --> Medium precission, medium reapeatability, medium current of 1.2muA
        precission = 2  --> High precission, high reapeatability, high current of 2.2muA
    */

   if(precission == 0) writeSensor1Byte(COMMAND_TRH_LOW_PREC);
   if(precission == 1) writeSensor1Byte(COMMAND_TRH_MED_PREC);
   if(precission == 2) writeSensor1Byte(COMMAND_TRH_HIGH_PREC);

   delay(10);

   uint8_t buffer [6];

   readSensorBytes(buffer, 6);
   //Data arrived in the buffer MSB first

   uint16_t rawTemp = *(buffer + 1) + *(buffer) * 256;
   uint16_t rawRH = *(buffer + 4) + *(buffer + 3) * 256;

   tempReadOut = -45 + 175 * ( rawTemp / 65535.0);
   RHReadOut = -6 + 125 * ( rawTemp / 65535.0);

   //crop the RH within 0% and 100%
   if(RHReadOut < 0 ) RHReadOut = 0;
   if(RHReadOut > 100 ) RHReadOut = 100;
}

void SHT40::softReset()
{
    Sensor::writeSensor1Byte(COMMAND_TRH_RESET);
    delay(10);
}