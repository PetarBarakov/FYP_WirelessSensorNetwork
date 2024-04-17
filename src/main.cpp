#include "header.h"

#define pinSDA 6 
#define pinSCL 7 

#define SHT40_ADDRESS 0x44

// publisherBLE node1BLE("FYP_SensorNode0");
SHT40 TRHSensor(SHT40_ADDRESS);

void setup() {
  Serial.begin(115200);

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
  double temp = 2, rh= 1;

  TRHSensor.readTempHumid(temp, rh);

  Serial.printf("The temperature is: %f \t The humidity is: %f\n", temp, rh);
  delay(2000);
}
