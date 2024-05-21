#include "ADS1292.h"

ADS1292::ADS1292(uint8_t ECG_SPI_CS) {
    SPI_CS = ECG_SPI_CS;
}

void ADS1292::setConfigReg1(uint8_t sampleRate)
{
    
    uint8_t commandBytes [2] = {ADS1292_WREG | ADS1292_CONFIG1_REG, 0x00};



    digitalWrite(SPI_CS, LOW);
    SPI.transfer(commandBytes[0]);
    // delay(2);
    SPI.transfer(commandBytes[1]);
    // delay(2);

    SPI.transfer(0b011);
    // delay(2);

    digitalWrite(SPI_CS, HIGH);
}

void ADS1292::readConfigReg1() {
    

    digitalWrite(SPI_CS, LOW);

    //stop data reading
    SPI.transfer(0x11);

    uint8_t txBytes [2] = {ADS1292_RREG | ADS1292_CONFIG1_REG, 0x00};

    uint8_t rxByte = 0x00;
    SPI.transfer(txBytes[0]);
    // delay(2);
    SPI.transfer(txBytes[1]);
    delay(2);
    rxByte = SPI.transfer(0x00);
    // delay(2);
    
    digitalWrite(SPI_CS, HIGH);

    Serial.printf("Config Reg 1: %02X\n", rxByte);

    // SPI.endTransaction();
}

void ADS1292::reset()
{

}