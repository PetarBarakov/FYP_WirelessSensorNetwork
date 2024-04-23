#include "MAX30102.h"

void MAX30102::fifoConfig()
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

void MAX30102::modeConfig()
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

void MAX30102::ledCurrentConfig()
{
    /*The 1 byte register value can be found with the following formula:
    regCurrent = typCurrent / 0.2;

    Input value needs to be between 0.2 and 51 mA
    */

    uint8_t ledRegCurrentByte = typCurrent / 0.2;
    writeToReg(LED1_PA_REG, ledRegCurrentByte);
    writeToReg(LED2_PA_REG, ledRegCurrentByte);

}

void MAX30102::SpO2Config()
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

MAX30102::MAX30102 (uint8_t PPG_sensorAddress,
                    uint8_t PPG_sampleAverage,
                    uint8_t PPG_mode,
                    uint8_t PPG_typCurrent,
                    uint8_t PPG_SpO2ADCRange,
                    uint8_t PPG_SpO2SampleRate,
                    uint8_t PPG_SpO2PulseWidth
                    ) : Sensor(PPG_sensorAddress)
{
    sampleAverage = PPG_sampleAverage;
    mode = PPG_mode;
    typCurrent = PPG_typCurrent;
    SpO2ADCRange = PPG_SpO2ADCRange;
    SpO2SampleRate = PPG_SpO2SampleRate;
    SpO2PulseWidth = PPG_SpO2PulseWidth;

    fifoConfig();      //In number of samples between 1 and 32
    modeConfig();               //Mode 0: Heart Rate, Mode 1: SpO2, Mode 2: Multi-LED  
    ledCurrentConfig();   //The typical currnet should be im mA
    SpO2Config();
}

void MAX30102::clearFIFO()
{
    //Loose all data currently stored in the FIFO

    writeToReg(FIFO_WR_PTR, 0x00);
    writeToReg(FIFO_RD_PTR, 0x00);
    writeToReg(FIFO_OVF_COUNTER, 0x00);
}

void MAX30102::readStatus()
{  
    Serial.printf("------ Control Vairables -----\n");
    Serial.printf("The sample average is %d samples\n", sampleAverage);
    Serial.printf("The Mode is %d \n", mode);
    Serial.printf("The LED current is %d mA\n", typCurrent);
   
   //TODO: read directly from the registers to see what has been actually programmed
}

void MAX30102::rawSpO2Read(uint32_t& redSampleRaw, uint32_t& irSampleRaw)
{
    // fifoConfig();
    modeConfig();
    ledCurrentConfig();
    // SpO2Config();

    uint8_t rdPointer, wrPointer;

    writeSensor1Byte(FIFO_RD_PTR);
    readSensorBytes(&rdPointer, 1);

    writeSensor1Byte(FIFO_WR_PTR);
    readSensorBytes(&wrPointer, 1);

    uint8_t numSamples = 0;

    if(rdPointer <= wrPointer) numSamples = wrPointer - rdPointer;
    else numSamples = 32 - (wrPointer - rdPointer);
    
    if(numSamples == 0) Serial.println("No samples in the FIFO");

    // Serial.printf("Write pointer %01X\n", wrPointer);
    // Serial.printf("Read pointer %01X\n", rdPointer);

    while (numSamples > 0)
    {
        // Serial.printf("Num of samples %f\n", numSamples);  
        numSamples --;

        //one sample read in Sp02 mode
        uint8_t rxBuffer [6];
    
        writeSensor1Byte(FIFO_DATA);
        readSensorBytes(rxBuffer, 6);

        // uint8_t redSampleRaw [3] = {rxBuffer[0], rxBuffer[1], rxBuffer[2]};
        // uint8_t irSampleRaw [3] = {rxBuffer[3], rxBuffer[4], rxBuffer[5]};
        redSampleRaw = 65536 * rxBuffer[0] + 256 * rxBuffer[1] + rxBuffer[2];
        irSampleRaw =  65536 * rxBuffer[3] + 256 * rxBuffer[4] + rxBuffer[5];

    }

}

void MAX30102::SpO2read()
{   
    uint32_t redSampleRaw = 0;
    uint32_t irSampleRaw = 0;

    rawSpO2Read(redSampleRaw, irSampleRaw);

    // Serial.printf("Red LED reading: %06X \t The IR LED reading: %06X\n", redSampleRaw, irSampleRaw);
    // Serial.printf("Red LED reading: %d \t The IR LED reading: %d\n", redSampleRaw, irSampleRaw);

    Serial.printf("Red: %d \t IR: %d\n", redSampleRaw, irSampleRaw);
}