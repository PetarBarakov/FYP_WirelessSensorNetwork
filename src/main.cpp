#include "header.h"

// I2C GPIO pins
#define pinSDA 6 
#define pinSCL 7 

//Uncomment the desired ESP to be programmed
// #define ESP32_SENSORS_MEASUREMENT
//#define ESP32_POWER_MEASUREMENT

#ifdef ESP32_SENSORS_MEASUREMENT

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



#endif //ESP32_SENSORS_MEASUREMENT


//Code for programmig the power measurement side of the Development Board
#ifdef ESP32_POWER_MEASUREMENT

#define LARGE_ADCRANGE 0
#define SMALL_ADCRANGE 1

//INA236A sensors:
#define CURRENT_SENSE_INPUT_ADDRESS 0x42  //Small Resolution and ADC Range
#define CURRENT_SENSE_PPG_ADDRESS 0x41  //Large Resolution and ADC Range

//INA228 sensors:
#define CURRENT_SENSE_ESP_ADDRESS 0x40  //Large Resolution and ADC Range
#define CURRENT_SENSE_SENSORS_ADDRESS 0x45  //Large Resolution and ADC Range

#define INPUT_CURRENT_SENSE_RESISTANCE 130 //[mOhms] Resistor for the input power measurements
#define PPG_CURRENT_SENSE_RESISTANCE 150 //[mOhms] Resistor for the PPG power measurements
#define ESP_CURRENT_SENSE_RESISTANCE 100 //[mOhms] Resistor for the ESP power measurements
#define SENSORS_CURRENT_SENSE_RESISTANCE 649 //[mOhms] Resistor for the Sensors power measurements


INA236 InputCurrentSense(CURRENT_SENSE_INPUT_ADDRESS);
INA236 PPGCurrentSense(CURRENT_SENSE_PPG_ADDRESS);

INA228 ESPCurrentSense(CURRENT_SENSE_ESP_ADDRESS);
INA228 SensorsCurrentSense(CURRENT_SENSE_SENSORS_ADDRESS);

void setup() {
  //Initialise Serial communication
  Serial.begin(115200);

  //Initialise I2C communication
  Wire.setPins(pinSDA, pinSCL);
  Wire.begin();

  InputCurrentSense.init(LARGE_ADCRANGE, 64, INPUT_CURRENT_SENSE_RESISTANCE);
  PPGCurrentSense.init(SMALL_ADCRANGE, 64, PPG_CURRENT_SENSE_RESISTANCE);
  ESPCurrentSense.init(SMALL_ADCRANGE, 64, ESP_CURRENT_SENSE_RESISTANCE);
  SensorsCurrentSense.init(SMALL_ADCRANGE, 64, SENSORS_CURRENT_SENSE_RESISTANCE);

}

void loop() {
  // I2CSearchAddr();
  double shuntVoltage = 0;
  uint32_t busVoltage = 0;
  uint32_t power = 0;
  double current = 0;

  InputCurrentSense.readShuntVoltage(shuntVoltage);
  InputCurrentSense.readBusVoltage(busVoltage);
  InputCurrentSense.readPower(power);
  InputCurrentSense.readCurrent(current);

  Serial.println("-------------Input Current Sensor:--------------");
  Serial.printf("Shunt Voltage: %f\n", shuntVoltage);
  Serial.printf("Bus Voltage: %d\n", busVoltage);
  Serial.printf("Power: %d\n", power);
  Serial.printf("Current: %f\n", current);
  Serial.println("-----------------------------------------------");
  Serial.println();

  PPGCurrentSense.readShuntVoltage(shuntVoltage);
  PPGCurrentSense.readBusVoltage(busVoltage);
  PPGCurrentSense.readPower(power);
  PPGCurrentSense.readCurrent(current);

  Serial.println("-------------PPG Current Sensor:--------------");
  Serial.printf("Shunt Voltage: %f\n", shuntVoltage);
  Serial.printf("Bus Voltage: %d\n", busVoltage);
  Serial.printf("Power: %d\n", power);
  Serial.printf("Current: %f\n", current);
  Serial.println("-----------------------------------------------");
  Serial.println();

  ESPCurrentSense.readShuntVoltage(shuntVoltage);
  ESPCurrentSense.readBusVoltage(busVoltage);
  ESPCurrentSense.readPower(power);
  ESPCurrentSense.readCurrent(current);

  Serial.println("-------------ESP Current Sensor:--------------");
  Serial.printf("Shunt Voltage: %f\n", shuntVoltage);
  Serial.printf("Bus Voltage: %d\n", busVoltage);
  Serial.printf("Power: %d\n", power);
  Serial.printf("Current: %f\n", current);
  Serial.println("-----------------------------------------------");
  Serial.println();

  SensorsCurrentSense.readShuntVoltage(shuntVoltage);
  SensorsCurrentSense.readBusVoltage(busVoltage);
  SensorsCurrentSense.readPower(power);
  SensorsCurrentSense.readCurrent(current);

  Serial.println("-------------Sensors Current Sensor:--------------"); 
  Serial.printf("Shunt Voltage: %f\n", shuntVoltage);
  Serial.printf("Bus Voltage: %d\n", busVoltage);
  Serial.printf("Power: %d\n", power);
  Serial.printf("Current: %f\n", current);
  Serial.println("-----------------------------------------------");
  Serial.println();
  
  delay(2000);
}

#endif // ESP32_POWER_MEASUREMENT
