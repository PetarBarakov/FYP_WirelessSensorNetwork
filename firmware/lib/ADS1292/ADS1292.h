#ifndef ADS1292_H
#define ADS1292_H

#include <Arduino.h>
#include <SPI.h>

#define ECG_SPI_MISO 0
#define ECG_SPI_MOSI 3
#define ECG_SPI_SCK 1
#define ECG_SPI_CS 10

//Commands
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

//Registers
#define ADS1292_CONFIG1_REG 0x01
#define ADS1292_CONFIG2_REG 0x02
#define ADS1292_LOFF_REG 0x03
#define ADS1292_CH1SET_REG 0x04
#define ADS1292_CH2SET_REG 0x05
#define ADS1292_RLD_SENS_REG 0x06
#define ADS1292_LOFF_SENS_REG 0x07
#define ADS1292_LOFF_STAT_REG 0x08

// #define ADS1292_RESP1_REG 0x09
// #define ADS1292_RESP2_REG 0x0A
// #define ADS1292_GPIO_REG 0x0B

class ADS1292 {
    
    public:
    ADS1292();
    void readConfigReg1();

    private:
    
};

#endif