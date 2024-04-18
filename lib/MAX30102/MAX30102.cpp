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
    writeToReg(FIFO_CONFIG_REG, fifoConfigBits);
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

    writeToReg(MODE_CONFIG_REG, modeBits);


}

void MAX30102::ledCurrentConfig(uint8_t typCurrent)
{
    /*The 1 byte register value can be found with the following formula;
    regCurrent = typCurrent / 0.2;
    */

    uint8_t ledRegCurrentByte = typCurrent / 0.2;
    writeToReg(LED1_PA_REG, ledRegCurrentByte);
    writeToReg(LED2_PA_REG, ledRegCurrentByte);

}

void MAX30102::SpO2Config(uint8_t SpO2ADCRange, uint8_t SpO2SampleRate, uint8_t SpO2PulseWidth)
{
    //The ADC Range has values from 0 to 3, increasing in the LSB size in terms of drawn current
    
    /*The Sample Rate is according to the following look-up table:
        0b000       -->         50 samp/s
        0b001       -->         100 samp/s
        0b010       -->         200 samp/s
        0b011       -->         400 samp/s
        0b100       -->         800 samp/s
        0b101       -->         1000 samp/s
        0b110       -->         1600 samp/s
        0b111       -->         3200 samp/s
    */
   
    /*The Pulse Width corresponds to the followin resolutions of the SpO2 ADC:
        0b00        -->         15 bits
        0b01        -->         16 bits
        0b10        -->         17 bits
        0b11        -->         18 bits
    */

    uint8_t sampleRateBits  = 0b100;    //Default sample rate of 800 samples per second
    
    uint8_t sampleRateLookUp [8] = {(uint8_t) 50, (uint8_t) 100, (uint8_t) 200, (uint8_t) 400, (uint8_t) 800, (uint8_t) 1000, (uint8_t) 1600, (uint8_t) 3200};
    for (uint8_t i = 0; i < 8; i ++)
    {
        if (sampleRateLookUp[i] == SpO2SampleRate) sampleRateBits = i;
    }

    uint8_t pulseWidthBits = 0b11;      //Default resolution of 18 bits

    uint8_t pulseWidthLookUp [4] = {16, 16, 17, 18};
    for (uint8_t i = 0; i < 4; i ++)
    {
        if (pulseWidthLookUp[i] == SpO2PulseWidth) pulseWidthBits = i;
    }

    uint8_t regBuffer = pulseWidthBits + 4 * sampleRateBits + 32 * SpO2ADCRange;

    writeToReg(SPO2_CONFIG_REG, regBuffer);


}

MAX30102::MAX30102 (uint8_t sensorAddress,
                    uint8_t sampleAverage,
                    uint8_t mode,
                    uint8_t typCurrent,
                    uint8_t SpO2ADCRange,
                    uint8_t SpO2SampleRate,
                    uint8_t SpO2PulseWidth
                    ) : Sensor(sensorAddress)
{
    fifoConfig(sampleAverage);      //In number of samples between 1 and 32
    modeConfig(mode);               //Mode 0: Heart Rate, Mode 1: SpO2, Mode 2: Multi-LED  
    ledCurrentConfig(typCurrent);   //The typical currnet should be im mA
    SpO2Config(SpO2ADCRange, SpO2SampleRate, SpO2PulseWidth);
}

void MAX30102::clearFIFO()
{
    //Loose all data currently stored in the FIFO

    writeToReg(FIFO_WR_PTR, 0x00);
    writeToReg(FIFO_RD_PTR, 0x00);
    writeToReg(FIFO_OVF_COUNTER, 0x00);
}
