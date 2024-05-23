#ifndef LIS2DE12_H
#define LIS2DE12_H

#include <Arduino.h>
#include <Wire.h>
#include "SensorClass.h"

#define LIS2DE12_CTRL_REG1 0x20         // <-- power/sample mode
// #define LIS2DE12_CTRL_REG2 0x21      // <-- filter
// #define LIS2DE12_CTRL_REG3 0x22      // <-- interupt
#define LIS2DE12_CTRL_REG4 0x23         // <-- scale selection
#define LIS2DE12_CTRL_REG5 0x24         // <-- FIFO selection
// #define LIS2DE12_CTRL_REG6 0x25      // <-- interrupt
// #define LIS2DE12_REFERENCE 0x26      // <-- reference for interrupt
// #define LIS2DE12_STATUS_REG 0x27     // <-- status register
#define LIS2DE12_FIFO_READ_START 0x28   // <-- acceleration data from FIFO
#define LIS2DE12_OUT_X 0x29             // <-- acceleration X-axis NORMAL MODE 
#define LIS2DE12_OUT_Y 0x2B             // <-- acceleration Y-axis NORMAL MODE
#define LIS2DE12_OUT_Z 0x2D             // <-- acceleration Z-axis NORMAL MODE
#define LIS2DE12_FIFO_CTRL_REG 0x2E     // <-- FIFO control
#define LIS2DE12_FIFO_SRC_REG 0x2F      // <-- FIFO status
// #define LIS2DE12_ACT_THS 0x3E        // <-- activity threshold
// #define LIS2DE12_ACT_DUR 0x3F        // <-- activity duration

//FIFO MODES
#define FIFO_BYPASS_MODE 0x00
#define FIFO_FIFO_MODE 0x40
#define FIFO_STREAM_MODE 0x80
#define FIFO_STREAM_TO_FIFO_MODE 0xC0



class LIS2DE12 : public Sensor{
    
    public:
    LIS2DE12(uint8_t sensorAddress);
    void init(uint16_t sampleRate, uint8_t inputScale);
    void readAcceleration(double* x, double* y, double* z, uint8_t* numSamples);

    private:
    void setSampleRate(uint16_t sampleRate);
    void setScale(uint8_t inputscale);
    void setFIFO();
    void setFIFOMode(uint8_t mode);
    void readFifoData(int16_t* x, int16_t* y, int16_t* z, uint8_t* numSamples);
    void reset();


    uint8_t scale;
};

#endif