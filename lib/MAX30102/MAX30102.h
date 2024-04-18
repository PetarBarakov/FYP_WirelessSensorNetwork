#ifndef MAX30102_H
#define MAX30102_H

#include <Arduino.h>
#include <Wire.h>
#include "SensorClass.h"

// Interupt Registers
// All interupts are disabled by default
// #define INT_STAT1 0x00
// #define INT_STAT2 0x01
// #define INT_EN1 0x02
// #define INT_EN2 0x03

//FIFO Register
#define FIFO_WR_PTR 0x04
#define FIFO_OVF_COUNTER 0x05
#define FIFO_RD_PTR 0x06
#define FIFO_DATA 0x07

// Main Configuration Register Addresses
#define FIFO_CONFIG 0x08
#define MODE_CONFIG 0x09
#define SPO2_CONFIG 0x0A
#define LED1_PA 0x0C    //RED LED
#define LED2_PA 0x0D    //IR LED




class MAX30102 : public Sensor
{
    public:
    MAX30102(uint8_t sensorAddress, uint8_t sampleAverage, uint8_t mode);

    void SPO2read();
    void HRread();  

    private:
    void readFIFO();
    void clearFIFO();

    //Configiration functions
    void fifoConfig(uint8_t sampleAverage);
    void modeConfig(uint8_t mode);

};

#endif