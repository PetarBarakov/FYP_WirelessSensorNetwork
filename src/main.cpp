#include "header.h"

// I2C GPIO pins
#define pinSDA 6 
#define pinSCL 7 

// Sensor addresses
#define SHT40_ADDRESS 0x44
#define MAX30102_ADDRESS 0x57
#define SGP41_ADDRESS 0x59

// publisherBLE node1BLE("FYP_SensorNode0"); //Initialise and object for the BLE transmission
SHT40 TRHSensor(SHT40_ADDRESS); //Intialise a object to the temperature and relative humidity sensor

void setup() {
  //Initialise Serial communication
  Serial.begin(115200);

  //Initialise I2C communication
  Wire.setPins(pinSDA, pinSCL);
  Wire.begin();

  // I2CSearchInit();

  // node1BLE.BLEinit();
}

void loop() {
  // I2CSearchAddr();

  // static int value = 0;
  // char value [] = "Temp: 7";

  // node1BLE.BLEsendValue(value);
  double temp, rh;

  TRHSensor.readTempHumid(temp, rh);

  Serial.printf("The temperature is: %f \t The humidity is: %f\n", temp, rh);
  
  
  
  
  delay(2000);
}
