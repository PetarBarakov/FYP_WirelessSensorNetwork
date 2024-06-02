#ifndef ADS1292_H
#define ADS1292_H

#include <Arduino.h>
#include <SPI.h>
#include "ecgRespirationAlgo.h"

//Commands---------------------------------------------------------------
#define ADS1292_WAKEUP 0x02
#define ADS1292_STANDBY 0x04
#define ADS1292_RESET 0x06
#define ADS1292_START 0x08
#define ADS1292_STOP 0x0A
#define ADS1292_OFFSETCAL 0x1A

#define ADS1292_RDATAC 0x10
#define ADS1292_SDATAC 0x11
#define ADS1292_RDATA 0x12

#define ADS1292_RREG 0x20  //-->
// OR-ed with the register to be read. 
//Send a second byte with the number of registers to be read 000n nnnn

#define ADS1292_WREG 0x40  //-->
// OR-ed with the register to be written. 
//Send a second byte with the number of registers to be written 000n nnnn

//Registers---------------------------------------------------------------
#define ADS1292_CONFIG1_REG 0x01        //-> Conversion Mode and Data Rate
#define ADS1292_CONFIG2_REG 0x02     //-> Lead-off Detection Control
// #define ADS1292_LOFF_REG 0x03        //-> Lead-off Status
#define ADS1292_CH1SET_REG 0x04         //-> Channel 1 Settings
#define ADS1292_CH2SET_REG 0x05         //-> Channel 2 Settings
// #define ADS1292_RLD_SENS_REG 0x06    //-> Right Leg Drive Sense Selection
// #define ADS1292_LOFF_SENS_REG 0x07   //-> Lead-off Sense Selection
// #define ADS1292_LOFF_STAT_REG 0x08   //-> Lead-off Status

// #define ADS1292_RESP1_REG 0x09
// #define ADS1292_RESP2_REG 0x0A
// #define ADS1292_GPIO_REG 0x0B

class ADS1292 {
    
    public:
    ADS1292(uint8_t ECG_SPI_CS);
    void init(uint16_t sampleRate, uint8_t gain);
    void readData();

    private:
    void setConfigReg1(uint16_t sampleRate);
    void setChannel1(uint8_t gain);
    void setChannel2();
    void enableInternalReference(bool enable);
    // void readConfigReg1();

    //SPI help functions
    void writeRegister(uint8_t reg, uint8_t data);
    void sendCommand(uint8_t command);
    
    
    uint8_t SPI_CS;
    ecg_respiration_algorithm filterAndConv;
};

#endif