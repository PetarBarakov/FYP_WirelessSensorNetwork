#include "header.h"

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

ADS1292 ECGSensor(ECG_SPI_CS);


// SPIClass SPI_module();
// SPISettings spi_setting (1000000, MSBFIRST, SPI_MODE1);

void setup() {
  //Initialise Serial communication
  Serial.begin(115200);

  pinMode(ECG_SPI_CS, OUTPUT);
  digitalWrite(ECG_SPI_CS, HIGH);

  SPI.begin(ECG_SPI_SCK, ECG_SPI_MISO, ECG_SPI_MOSI, ECG_SPI_CS);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setFrequency(1000000);

  ECGSensor.init(500, 6);
}


void loop(){
  

// // ------------------ ECG Sensor ------------------

  ECGSensor.readData();
  delay(10);

}



#endif //ESP32_SENSORS_MEASUREMENT

