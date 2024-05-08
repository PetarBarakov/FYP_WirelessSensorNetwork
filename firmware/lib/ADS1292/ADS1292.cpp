#include "ADS1292.h"

ADS1292::ADS1292() {
    pinMode(ECG_SPI_CS, OUTPUT);
    digitalWrite(ECG_SPI_CS, HIGH);

    SPI.begin(ECG_SPI_SCK, ECG_SPI_MISO, ECG_SPI_MOSI, ECG_SPI_CS);
    // SPI.begin();
    // SPI.beginTransaction(SPISettings(ADS1292_SPI_FREQ, MSBFIRST, SPI_MODE1));
}

void ADS1292::readConfigReg1() {
    // SPI.beginTransaction(SPISettings(512000, MSBFIRST, SPI_MODE0));


    digitalWrite(ECG_SPI_CS, LOW);

    uint8_t txBytes [2] = {ADS1292_RREG | ADS1292_CONFIG1_REG, 0x01};

    uint8_t rxByte = 0x03;
    SPI.transfer(txBytes[0]);
    SPI.transfer(txBytes[1]);
    rxByte = SPI.transfer(0x00);
    
    digitalWrite(ECG_SPI_CS, HIGH);

    Serial.printf("Config Reg 1: %x\n", rxByte);

    // SPI.endTransaction();
}