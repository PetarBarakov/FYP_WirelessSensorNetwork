#include "header.h"

// I2C GPIO pins
#define pinSDA 6 
#define pinSCL 7 

#define LARGE_ADCRANGE 0
#define SMALL_ADCRANGE 1

//INA236A sensors:
#define CURRENT_SENSE_INPUT_ADDRESS 0x42  //Small Resolution and ADC Range
#define CURRENT_SENSE_PPG_ADDRESS 0x41  //Large Resolution and ADC Range

//INA228 sensors:
#define CURRENT_SENSE_ESP_ADDRESS 0x40  //Large Resolution and ADC Range
#define CURRENT_SENSE_SENSORS_ADDRESS 0x45  //Large Resolution and ADC Range

#define INPUT_CURRENT_SENSE_RESISTANCE 130 //Resistor for the input power measurements


INA236 InputcurrentSense(CURRENT_SENSE_INPUT_ADDRESS);

void setup() {
  //Initialise Serial communication
  Serial.begin(115200);

  //Initialise I2C communication
  Wire.setPins(pinSDA, pinSCL);
  Wire.begin();

  InputcurrentSense.init(LARGE_ADCRANGE, 64, INPUT_CURRENT_SENSE_RESISTANCE);

}

void loop() {
  // I2CSearchAddr();
  double shuntVoltage = 0;
  uint16_t busVoltage = 0;
  uint16_t power = 0;
  double current = 0;

  InputcurrentSense.readShuntVoltage(shuntVoltage);
  InputcurrentSense.readBusVoltage(busVoltage);
  InputcurrentSense.readPower(power);
  InputcurrentSense.readCurrent(current);

  Serial.printf("Shunt Voltage from sensor 1: %f\n", shuntVoltage);
  Serial.printf("Bus Voltage from sensor 1: %d\n", busVoltage);
  Serial.printf("Power from sensor 1: %d\n", power);
  Serial.printf("Current from sensor 1: %f\n", current);
  Serial.println();
  
  delay(2000);
}
