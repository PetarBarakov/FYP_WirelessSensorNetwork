#include <header.h>

#define pinSDA 6 
#define pinSCL 7 

#define LED1 4

void setup() {
  Serial.begin(115200);

  I2CSearchInit();
}

void loop() {
  I2CSearchAddr();
}