// #ifndef I2C_test.h
// #define I2C_test.h

#include <Arduino.h>
#include <Wire.h>
#include <I2C_test.h>

void I2CSearchInit()
{
    Wire.setPins(pinSDA, pinSCL);
    Wire.begin();

    pinMode(LED1, OUTPUT);
}

void I2CSearchAddr()
{
     byte error, address;
  int nDevices = 0;

  digitalWrite(LED1, HIGH);
  delay(1000);
  digitalWrite(LED1, LOW);
  delay(1000);

  Serial.println("Scanning for I2C devices (using I2C_test library) ...");
  for(address = 0x01; address < 0x7f; address++){
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0){
      Serial.printf("I2C device found at address 0x%02X\n", address);
      nDevices++;
    } else if(error != 2){
      Serial.printf("Error %d at address 0x%02X\n", error, address);
    }
  }
  if (nDevices == 0){
    Serial.println("No I2C devices found");
  }
}

// #endif