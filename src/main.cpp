#include "header.h"

// I2C GPIO pins
#define pinSDA 6 
#define pinSCL 7 

// Sensor addresses
#define SHT40_ADDRESS 0x44
#define MAX30102_ADDRESS 0x57
#define SGP41_ADDRESS 0x59

// publisherBLE node1BLE("FYP_SensorNode0"); //Initialise an object for the BLE transmission
SHT40 TRHSensor(SHT40_ADDRESS); //Intialise an object to the temperature and relative humidity sensor

//Initialise an object for the PPG sensor
MAX30102 PPGSensor(MAX30102_ADDRESS);

void setup() {
  //Initialise Serial communication
  Serial.begin(115200);

  //Initialise I2C communication
  Wire.setPins(pinSDA, pinSCL);
  Wire.begin();

  // delay(5000);

  PPGSensor.init( (uint8_t) 8,                //sampleAverage
                  (uint8_t) 1,                //mode
                  (uint8_t) 12,                //typCurrent
                  (uint8_t) 0b10,             //SpO2ADCRange corresponding to bits
                  (uint16_t) 100,              //SpO2SampleRate
                  (uint8_t) 18                //SpO2PulseWidth
                );  

  // I2CSearchInit();

  // node1BLE.BLEinit();

  // PPGSensor.clearFIFO();

}

void loop() {
  // I2CSearchAddr();

  // static int value = 0;
  // char value [] = "Temp: 7";

  // node1BLE.BLEsendValue(value);
  // double temp, rh;

  // TRHSensor.readTempHumid(temp, rh);
  uint32_t redSampleBuffer [32];
  uint32_t irSampleBuffer [32];
  uint8_t userBuffer;
  
  PPGSensor.SpO2read(redSampleBuffer, irSampleBuffer, userBuffer);


  for(uint8_t i = 0; i < userBuffer; i++)
  {
    Serial.printf("Red: %d \t IR: %d\n", *(redSampleBuffer + i), *(irSampleBuffer + i));
  }
  
  Serial.println("....................");
  Serial.printf("Number of samples: %d\n", userBuffer);
  delay(3000);
}
