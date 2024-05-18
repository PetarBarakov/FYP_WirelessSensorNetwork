#include "MAX30102.h"

MAX30102::MAX30102(uint8_t sensorAddress) : Sensor (sensorAddress)
{

}

MAX30102::~MAX30102()
{
    delete [] redSampleBuffer;
    delete [] irSampleBuffer;
}

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
    /*The 1 byte register value can be found with the following formula:
    regCurrent = typCurrent / 0.2;

    Input value needs to be between 0.2 and 51 mA
    */

    uint8_t ledRegCurrentByte = typCurrent / 0.2;
    writeToReg(LED1_PA_REG, ledRegCurrentByte);
    writeToReg(LED2_PA_REG, ledRegCurrentByte);

}

void MAX30102::SpO2Config(uint8_t SpO2ADCRange, uint16_t SpO2SampleRate, uint8_t SpO2PulseWidth)
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

   //TODO: check the sample rate and pulse width ratio from the datasheet again

    uint8_t sampleRateBits  = 0b100;    //Default sample rate of 800 samples per second
    
    uint16_t sampleRateLookUp [8] = {50, 100, 200, 400, 800, 1000, 1600, 3200};
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

void MAX30102::clearFIFO()
{
    //Loose all data currently stored in the FIFO

    writeToReg(FIFO_WR_PTR, 0x00);
    writeToReg(FIFO_RD_PTR, 0x00);
    writeToReg(FIFO_OVF_COUNTER, 0x00);
}

void MAX30102::reset()
{
    uint8_t reset_val = 0b01000000;
    writeToReg(MODE_CONFIG_REG, reset_val);
}

void MAX30102::init (uint8_t sampleAverage,
                     uint8_t mode,
                     uint8_t typCurrent,
                     uint8_t SpO2ADCRange,
                     uint16_t SpO2SampleRate,
                     uint8_t SpO2PulseWidth
                     )
{
    reset();
    delay(10);
    clearFIFO();
    delay(10);

    fifoConfig(sampleAverage);      //In number of samples between 1 and 32
    modeConfig(mode);               //Mode 0: Heart Rate, Mode 1: SpO2, Mode 2: Multi-LED  
    ledCurrentConfig(typCurrent);   //The typical currnet should be im mA
    SpO2Config(SpO2ADCRange, SpO2SampleRate, SpO2PulseWidth);

    writeToReg(INT_EN1, 0b10000000); //enable the overflow interrupt

    // readStatus();
}

void MAX30102::readStatus()
{  
    Serial.printf("------ Control Vairables -----\n");
    
    uint8_t control_buffer;

    writeSensor1Byte(FIFO_CONFIG_REG);
    readSensorBytes(&control_buffer, 1);
    Serial.printf("FIFO Config: %02X\n", control_buffer);

    writeSensor1Byte(MODE_CONFIG_REG);
    readSensorBytes(&control_buffer, 1);
    Serial.printf("Mode Config: %02X\n", control_buffer);

    writeSensor1Byte(SPO2_CONFIG_REG);
    readSensorBytes(&control_buffer, 1);
    Serial.printf("SpO2 Config: %02X\n", control_buffer);

    writeSensor1Byte(LED1_PA_REG);
    readSensorBytes(&control_buffer, 1);
    Serial.printf("LED1 Current: %02X\n", control_buffer);
   
    writeSensor1Byte(LED2_PA_REG);
    readSensorBytes(&control_buffer, 1);
    Serial.printf("LED2 Current %02X\n", control_buffer);
}
/*
void MAX30102::rawSpO2Read(uint32_t *redSampleRaw, uint32_t *irSampleRaw, uint8_t &usedBuffer)
{
    uint8_t rdPointer, wrPointer;

    writeSensor1Byte(FIFO_RD_PTR);
    readSensorBytes(&rdPointer, 1);

    writeSensor1Byte(FIFO_WR_PTR);
    readSensorBytes(&wrPointer, 1);

    uint8_t numSamples = 0;

    if(rdPointer <= wrPointer) numSamples = wrPointer - rdPointer;
    else numSamples = 32 - (rdPointer - wrPointer);

    if(numSamples == 0) Serial.println("No samples in the FIFO");
    if(numSamples > 32)
    {
        // numSamples = 32;
        Serial.println("-----------------ERROR: More samples that allowed-----------------");
    }
    // Serial.printf("Write pointer %d\n", wrPointer);
    // Serial.printf("Read pointer %d\n", rdPointer);

    // //Read the interupt register
    // writeSensor1Byte(INT_STAT1);
    // uint8_t regBuffer;
    // readSensorBytes(&regBuffer, 1);
    // Serial.printf("The interrupt register value: %02X\n", regBuffer);
    // Serial.printf("The interrupt register value: %d\n", (regBuffer & 0b10000000) >> 7);

    // //Read the overrlow register
    // writeSensor1Byte(FIFO_OVF_COUNTER);
    // readSensorBytes(&regBuffer, 1);
    // Serial.printf("The overflow counter register value: %d\n", regBuffer);



    usedBuffer = 0;

    while (numSamples > 0)
    {
        //one sample read in Sp02 mode
        uint8_t rxBuffer [6];
    
        writeSensor1Byte(FIFO_DATA);
        readSensorBytes(rxBuffer, 6);

        *(redSampleRaw + usedBuffer) = 65536 * rxBuffer[0] + 256 * rxBuffer[1] + rxBuffer[2];
        *(irSampleRaw  + usedBuffer) =  65536 * rxBuffer[3] + 256 * rxBuffer[4] + rxBuffer[5];
        
        numSamples --;
        usedBuffer ++;
    }

}
*/

void MAX30102::rawSpO2ReadOneSample(uint32_t *redSampleRaw, uint32_t *irSampleRaw)
{   
    //wait for a sample to be present
    uint8_t rdPointer, wrPointer;
    uint8_t numSamples = 0;

    while (numSamples == 0)
    {
        writeSensor1Byte(FIFO_RD_PTR);
        readSensorBytes(&rdPointer, 1);

        writeSensor1Byte(FIFO_WR_PTR);
        readSensorBytes(&wrPointer, 1);

        if(rdPointer <= wrPointer) numSamples = wrPointer - rdPointer;
        else numSamples = 32 - (rdPointer - wrPointer);

        delay(1);
    }
    //one sample read in Sp02 mode
    uint8_t rxBuffer [6];

    writeSensor1Byte(FIFO_DATA);
    readSensorBytes(rxBuffer, 6);

    *redSampleRaw = 65536 * rxBuffer[0] + 256 * rxBuffer[1] + rxBuffer[2];
    *irSampleRaw  =  65536 * rxBuffer[3] + 256 * rxBuffer[4] + rxBuffer[5];
}

void MAX30102::SpO2andHRread(int32_t *HR, int32_t *spo2)
{   
    static bool firstMeasurement = true;

    uint8_t bufferSize = 100;

    if(firstMeasurement)
    {
        redSampleBuffer = new uint32_t [bufferSize];
        irSampleBuffer = new uint32_t [bufferSize];
        
        uint8_t numSamplesInBuffer = 0;

        while (numSamplesInBuffer < bufferSize)
        {   
            rawSpO2ReadOneSample(redSampleBuffer + numSamplesInBuffer, irSampleBuffer + numSamplesInBuffer);
            // Serial.printf("RED: %d \t IR: %d \n ", *(redSampleBuffer + numSamplesInBuffer), *(irSampleBuffer + numSamplesInBuffer));
            numSamplesInBuffer ++;        
        }
        firstMeasurement = false;
    }


    int8_t validSPO2, validHeartRate;
    int32_t spo2_new, HR_new;

    maxim_heart_rate_and_oxygen_saturation(irSampleBuffer, bufferSize, redSampleBuffer, &spo2_new, &validSPO2, &HR_new, &validHeartRate);
    if(validSPO2 == true) *spo2 = spo2_new;
    // else Serial.println("Invalid SpO2");
    
    if(validHeartRate == true) *HR = HR_new;
    // else Serial.println("Invalid HR");
    
    // Serial.printf("---HR: %d \t SpO2: %d \t ", *HR, *spo2);
    // Serial.printf("---Valid HR: %d \t Valid SpO2: %d \n", validHeartRate, validSPO2);

    
    for (byte i = 25; i < bufferSize; i++)
    {
      redSampleBuffer[i - 25] = redSampleBuffer[i];
      irSampleBuffer[i - 25] = irSampleBuffer[i];
    }

    //take 25 sets of samples before calculating the heart rate.
    uint8_t numSamplesAdded = bufferSize  - 25;

    while (numSamplesAdded < bufferSize)
    {   
        rawSpO2ReadOneSample(redSampleBuffer + numSamplesAdded, irSampleBuffer + numSamplesAdded);
        // Serial.printf("RED: %d \t IR: %d \n ", *(redSampleBuffer + numSamplesAdded), *(irSampleBuffer + numSamplesAdded));
        numSamplesAdded ++;        
    }
}
/*
double MAX30102::HRread(uint32_t *irSampleBuffer)
{
    uint32_t redSampleBuffer;
    // uint32_t irSampleBuffer;

    static long lastBeat = 0;

    const uint8_t RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
    static double rates[RATE_SIZE]; //Array of heart rates
    static uint8_t rateSpot = 0;
    

    double beatsPerMinute = 0;
    static uint16_t beatAvg;      

    rawSpO2ReadOneSample(&redSampleBuffer, irSampleBuffer);
    
    if (checkForBeat(*irSampleBuffer) == true)
    {
        //We sensed a beat!
        long delta = millis() - lastBeat;
        lastBeat = millis();

        beatsPerMinute = 60 / (delta / 1000.0);

        if (beatsPerMinute < 255 && beatsPerMinute > 20)
        {
            rates[rateSpot++] = beatsPerMinute; //Store this reading in the array
            rateSpot %= RATE_SIZE; //Wrap variable

            //Take average of readings
            beatAvg = 0;
            for (byte x = 0 ; x < RATE_SIZE ; x++)
                beatAvg += rates[x];
            beatAvg /= RATE_SIZE;
        }
    }



    // if(irSampleBuffer < 50000)
    //     Serial.print(" No finger?\n");
    // else
    // {
    //     Serial.print("IR=");
    //     Serial.print(irSampleBuffer);
    //     Serial.print(", BPM=");
    //     Serial.print(beatsPerMinute);
    //     Serial.print(", Avg BPM=");
    //     Serial.print(beatAvg);
    //     Serial.print("\n");

    // }

    // return beatsPerMinute;
    return beatAvg;
}
*/