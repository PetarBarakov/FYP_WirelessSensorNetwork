#include "header.h"

// I2C GPIO pins
#define pinSDA 6 
#define pinSCL 7 

void setup() {
  //Initialise Serial communication
  Serial.begin(115200);

  //Initialise I2C communication
  Wire.setPins(pinSDA, pinSCL);
  Wire.begin();

}

void loop() {
  // I2CSearchAddr();
}
