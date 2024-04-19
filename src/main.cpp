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
MAX30102 PPGSensor( MAX30102_ADDRESS, //sensorAddress
                    8,                //sampleAverage
                    1,                //mode
                    12,                //typCurrent
                    0b10,             //SpO2ADCRange in bits
                    800,              //SpO2SampleRate
                    18                //SpO2PulseWidth
                    );  


void setup() {
  //Initialise Serial communication
  Serial.begin(115200);

  //Initialise I2C communication
  Wire.setPins(pinSDA, pinSCL);
  Wire.begin();

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

  // Serial.printf("The temperature is: %f \t The humidity is: %f\n", temp, rh);
  
  PPGSensor.SPO2read();

  
  
  delay(2000);
}
