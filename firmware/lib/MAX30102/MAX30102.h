#ifndef MAX30102_H
#define MAX30102_H

#include <Arduino.h>
#include <Wire.h>
#include "SensorClass.h"
#include "heartRate.h"

// Interupt Registers
// All interupts are disabled by default
#define INT_STAT1 0x00
// #define INT_STAT2 0x01
#define INT_EN1 0x02
// #define INT_EN2 0x03

//FIFO Register
#define FIFO_WR_PTR 0x04
#define FIFO_OVF_COUNTER 0x05
#define FIFO_RD_PTR 0x06
#define FIFO_DATA 0x07

// Main Configuration Register Addresses
#define FIFO_CONFIG_REG 0x08
#define MODE_CONFIG_REG 0x09
#define SPO2_CONFIG_REG 0x0A
#define LED1_PA_REG 0x0C    //RED LED
#define LED2_PA_REG 0x0D    //IR LED

#define FIFO_LENGTH 32


class MAX30102 : public Sensor
{
    public:
    MAX30102 (uint8_t sensorAddress);

    void init ( uint8_t sampleAverage,
                uint8_t mode,
                uint8_t typCurrent,
                uint8_t SpO2ADCRange,
                uint16_t SpO2SampleRate,
                uint8_t SpO2PulseWidth
              );

    void SpO2read(uint32_t *redSampleBuffer, uint32_t *irSampleBuffer, uint8_t &usedBuffer);
    double HRread(uint32_t *irSampleBuffer);  

    void clearFIFO();
    void reset();
    void readStatus();

    private:
    void rawSpO2Read(uint32_t *redSampleRaw, uint32_t *irSampleRaw, uint8_t &usedBuffer);
    void rawSp02ReadOneSample(uint32_t *redSampleRaw, uint32_t *irSampleRaw);

    //Configiration functions
    void fifoConfig(uint8_t sampleAverage);
    void modeConfig(uint8_t mode);
    void ledCurrentConfig(uint8_t typCurrent);
    void SpO2Config(uint8_t SpO2ADCRange, uint16_t SpO2SampleRate, uint8_t SpO2PulseWidth);


};

#endif