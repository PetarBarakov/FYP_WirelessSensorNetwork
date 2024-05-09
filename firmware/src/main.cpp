// #include "header.h"

// I2C GPIO pins
// #define pinSDA 6 
// #define pinSCL 7 

#ifdef ESP32_SENSORS_MEASUREMENT

#define SHT40_ADDRESS 0x44
#define MAX30102_ADDRESS 0x57
#define SGP41_ADDRESS 0x59
#define LIS2DE12_ADDRESS 0x18

#include <SPI.h>
#include <Arduino.h>
#define ECG_SPI_MISO 0
#define ECG_SPI_MOSI 3
#define ECG_SPI_SCK 1
#define ECG_SPI_CS 10

SPIClass SPI_module(HSPI);
SPISettings spi_setting (1000000, MSBFIRST, SPI_MODE1);

void setup() {
  //Initialise Serial communication
  Serial.begin(115200);

  // SPI.begin(ECG_SPI_SCK, ECG_SPI_MISO, ECG_SPI_MOSI, ECG_SPI_CS);
  SPI_module.begin(ECG_SPI_SCK, ECG_SPI_MISO, ECG_SPI_MOSI, ECG_SPI_CS);
  pinMode(ECG_SPI_CS, OUTPUT);
  digitalWrite(ECG_SPI_CS, HIGH);
}


void loop(){
  

// ------------------ ECG Sensor ------------------

  // ECGSensor.readConfigReg1();
  digitalWrite(ECG_SPI_CS, LOW);
  delay(2);
  digitalWrite(ECG_SPI_CS, HIGH);
  delay(2);
  digitalWrite(ECG_SPI_CS, LOW);

  uint8_t txBytes [2] = {0x20 | 0x01, 0x00};

  uint8_t rxByte = 0x03;

  // SPI_module.beginTransaction(spi_setting);
  // delay(2);
  // SPI_module.transfer(txBytes[0]);
  // // delay(2);
  // SPI_module.transfer(txBytes[1]);


  //Wake up
  SPI_module.transfer(0x02);
  //startCommand
  SPI_module.transfer(0x08);

  // delay(2);
  for(int i = 0; i < 10; i++){
    rxByte = SPI_module.transfer(0xFF);
    Serial.printf("Config Reg 1: %02x\n", rxByte);
  }
  // rxByte = SPI_module.transfer(0xFF);
    
  // delay(2);
  // digitalWrite(ECG_SPI_CS, HIGH);

  // Serial.printf("Config Reg 1: %02x\n", rxByte);

  // SPI_module.endTransaction();


  delay(2000);

}



#endif //ESP32_SENSORS_MEASUREMENT

