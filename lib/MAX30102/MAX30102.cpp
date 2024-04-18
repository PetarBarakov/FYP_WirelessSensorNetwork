#include "MAX30102.h"

void MAX30102::fifoConfig(uint8_t sampleAverage)
{
    //Setup the FIFO configuration

    //Chose a sampling average:
    /*  The possible averages of the fifo samples are:
    0b000                     -->        1  sample
    0b001                     -->        2  samples
    0b010                     -->        4  samples
    0b011                     -->        8  samples
    0b100                     -->        16 samples
    0b101, 0b110, 0b111       -->        32 samples
    */

    uint8_t fifoAFullBits = 0b0000; //Set the FIFO almost full value 
    bool fifoROBit = 0b1;           //Set the FIFO roll over
    uint8_t smpAvgBits = 0b011;     //default averaging of 8 samples;

    switch (sampleAverage)
    {
    case 1:
        smpAvgBits = 0b000;
        break;
    
    case 2:
        smpAvgBits = 0b001;
        break;
    
    case 4:
        smpAvgBits = 0b010;
        break;
    
    case 8:
        smpAvgBits = 0b011;
        break;
    
    case 16:
        smpAvgBits = 0b100;
        break;
    
    case 32:
        smpAvgBits = 0b101;
        break;
    
    default:
        break;
    }

    //Calculate and write the register value
    uint8_t fifoConfigBits = fifoAFullBits + 16 * fifoROBit + 32 * smpAvgBits;
    writeToReg(FIFO_CONFIG, fifoConfigBits);
}

void MAX30102::modeConfig(uint8_t mode)
{
    //Setup the mode of measurements

    /*  Available modes:
    Mode 0:      0b010       -->         Heart Rate mode (Red LED only)
    Mode 1:      0b011       -->         SpO2 mode (Red and IR LEDs)
    Mode 2:      0b111       -->         Multi-LED mode (Red and IR LEDs)
    */

    uint8_t modeBits = 0b011; //default mode is Sp02
    
    if(mode == 0) modeBits = 0b010;
    else if(mode == 2) modeBits = 0b111;

    writeToReg(MODE_CONFIG, modeBits);


}

MAX30102::MAX30102 (uint8_t sensorAddress,
                    uint8_t sampleAverage,
                    uint8_t mode
                    ) : Sensor(sensorAddress)
{
    fifoConfig(sampleAverage);
    modeConfig(mode);
}

void MAX30102::clearFIFO()
{
    //Loose all data currently stored in the FIFO

    writeToReg(FIFO_WR_PTR, 0x00);
    writeToReg(FIFO_RD_PTR, 0x00);
    writeToReg(FIFO_OVF_COUNTER, 0x00);
}