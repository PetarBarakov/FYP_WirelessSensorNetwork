#include "ADS1292.h"

ADS1292::ADS1292(uint8_t ECG_SPI_CS) {
    SPI_CS = ECG_SPI_CS;
}

void ADS1292::init(uint16_t sampleRate, uint8_t gain)
{
    sendCommand(ADS1292_RESET);
    delay(10);
    sendCommand(ADS1292_SDATAC);
    delay(10);

    setConfigReg1(sampleRate);
    setChannel1(gain);
    setChannel2();
    // enableInternalReference(false);
}

void ADS1292::writeRegister(uint8_t reg, uint8_t data)
{
    uint8_t commandBytes [2] = {ADS1292_WREG | reg, 0x00};

    digitalWrite(SPI_CS, LOW);
    SPI.transfer(commandBytes[0]);
    SPI.transfer(commandBytes[1]);
    SPI.transfer(data);
    digitalWrite(SPI_CS, HIGH);
}

void ADS1292::sendCommand(uint8_t command)
{
    digitalWrite(SPI_CS, LOW);
    SPI.transfer(command);
    digitalWrite(SPI_CS, HIGH);
}

void ADS1292::setConfigReg1(uint16_t sampleRate)
{
    
    uint8_t commandBytes [2] = {ADS1292_WREG | ADS1292_CONFIG1_REG, 0x00};
    uint8_t sampleRateByte = 0b010;

    /*Sample rate --> bits[2:0]
    0b000       -->     125 SPS
    0b001       -->     250 SPS
    0b010       -->     500 SPS
    0b011       -->     1 kSPS
    0b100       -->     2 kSPS
    0b101       -->     4 kSPS
    0b110       -->     8 kSPS
    */

    switch (sampleRate)
    {
    case 125:
        sampleRateByte = 0b000;
        break;
    case 250:
        sampleRateByte = 0b001;
        break;
    case 500:
        sampleRateByte = 0b010;
        break;
    case 1000:
        sampleRateByte = 0b011;
        break;
    case 2000:
        sampleRateByte = 0b100;
        break;
    case 4000:
        sampleRateByte = 0b101;
        break;
    case 8000:
        sampleRateByte = 0b110;
        break;
    
    default:
        sampleRateByte = 0b010;
        break;
    }
   
    writeRegister(ADS1292_CONFIG1_REG, sampleRateByte);
}

void ADS1292::setChannel1(uint8_t gain)
{
    uint8_t gainByte = 0b000;

    /*PGA Gain --> bits[6:4]
    0b000       -->     6
    0b001       -->     1
    0b010       -->     2
    0b011       -->     3
    0b100       -->     4
    0b101       -->     8
    0b110       -->     12
    0b111       -->     24
    */

    switch (gain)
    {
    case 6:
        gainByte = 0b000;
        break;
    case 1:
        gainByte = 0b001;
        break;
    case 2:
        gainByte = 0b010;
        break;
    case 3:
        gainByte = 0b011;
        break;
    case 4:
        gainByte = 0b100;
        break;
    case 8:
        gainByte = 0b101;
        break;
    case 12:
        gainByte = 0b110;
        break;
    case 24:
        gainByte = 0b111;
        break;
    
    default:
        gainByte = 0b000;
        break;
    }

    writeRegister(ADS1292_CH1SET_REG, gainByte);
}

void ADS1292::setChannel2()
{
    writeRegister(ADS1292_CH2SET_REG, 0b10000001);
}

void ADS1292::enableInternalReference(bool enable)
{
    if(enable) writeRegister(ADS1292_CONFIG2_REG, 0b10110000);
    else writeRegister(ADS1292_CONFIG2_REG, 0b10000000);
}

/*void ADS1292::readConfigReg1() {
    

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
}*/

void ADS1292::readData() {
    sendCommand(ADS1292_RDATA);

    uint8_t rxdata[9];

    digitalWrite(SPI_CS, LOW);
    for (int i = 0; i < 9; i++)
    {
        rxdata[i] = SPI.transfer(0x00);
    }
    digitalWrite(SPI_CS, HIGH);

    int32_t ch1_data = (int32_t) (rxdata[3] << 16 | rxdata[4] << 8 | rxdata[5]);

    Serial.printf("Data: %d\n", ch1_data);
    delay(10);
}