#ifndef SGP41_H
#define SGP41_H

#include <Arduino.h>
#include <Wire.h>
#include "SensorClass.h"

class SGP41 : public Sensor {

    public:
    SGP41(uint8_t sensorAddress);
    void readSample(double& SRAW_VOC, double& SRAW_NOX);
    void executeConditioning(uint16_t& SRAW_VOC_INTIAL);
    void selftest();


    private:
    void meaureRawSignal(uint16_t& SRAW_VOC, uint16_t& SRAW_NOX);
    void turnHeaterOff();
    void reset();
};

#endif