#include "header.h"

publisherBLE node1BLE("FYP_SensorNode0");

void setup() {
  Serial.begin(115200);

  I2CSearchInit();

  node1BLE.BLEinit();
}

void loop() {
  // I2CSearchAddr();

  // static int value = 0;
  char value [] = "Temp: 7";

  node1BLE.BLEsendValue(value);

  delay(1000);
}
