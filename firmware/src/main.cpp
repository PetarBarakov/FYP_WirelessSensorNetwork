#include "header.h"

// I2C GPIO pins
#define pinSDA 6 
#define pinSCL 7 

#ifdef ESP32_SENSORS_MEASUREMENT

#define SHT40_ADDRESS 0x44
#define MAX30102_ADDRESS 0x57
#define SGP41_ADDRESS 0x59
#define LIS2DE12_ADDRESS 0x18

// #include <SPI.h>
// #define ECG_SPI_MISO 0
// #define ECG_SPI_MOSI 3
// #define ECG_SPI_SCK 1
// #define ECG_SPI_CS 10

publisherBLE node1BLE("FYP_SensorNode0"); //Initialise an object for the BLE transmission
SHT40 TRHSensor(SHT40_ADDRESS); //Intialise an object to the temperature and relative humidity sensor

// //Initialise an object for the PPG sensor
MAX30102 PPGSensor(MAX30102_ADDRESS);

// //Initialise an object for the VOC sensor
// SGP41 VOCSensor(SGP41_ADDRESS);

// LIS2DE12 AccelSensor(LIS2DE12_ADDRESS);

// ADS1292 ECGSensor;

void setup() {
  //Initialise Serial communication
  Serial.begin(115200);

  //Initialise I2C communication
  Wire.setPins(pinSDA, pinSCL);
  Wire.begin();

  // delay(5000);

  PPGSensor.init( (uint8_t) 8,                //sampleAverage
                  (uint8_t) 1,                //mode
                  (uint8_t) 20,                //typCurrent [mA]
                  (uint8_t) 0b10,             //SpO2ADCRange corresponding to bits
                  (uint16_t) 800,              //SpO2SampleRate
                  (uint8_t) 18                //SpO2PulseWidth
                );  

  // uint16_t SRAW_VOC_INTIAL = 0;
  // VOCSensor.executeConditioning(SRAW_VOC_INTIAL);
  // Serial.printf("Initial SRAW VOC: %d\n", SRAW_VOC_INTIAL);

  // AccelSensor.init(100, 2); //Sampler rate of 100Hz and scale of 2g

  // I2CSearchInit();

  node1BLE.BLEinit();

  // PPGSensor.clearFIFO();

}

void loop() {
  // I2CSearchAddr();

  // static int value = 0;
  // char value [] = "Temp: 7";

  // node1BLE.BLEsendValue(value);

//------------ Temperature and Relative Humidity Sensor ------------

  // double temp, rh;
  // TRHSensor.readTempHumid(temp, rh);
  // char TRHmessage[32];
  // sprintf(TRHmessage, "%f,%f", temp, rh);

  // Serial.printf("TEMP: %f \t RH: %f \t", temp, rh);
  // node1BLE.BLEsendValue(TRHmessage);



  
//------------ PPG Sensor ------------

  uint32_t redSampleBuffer [32];
  uint32_t irSampleBuffer [32];
  uint8_t userBuffer;
  
  PPGSensor.SpO2read(redSampleBuffer, irSampleBuffer, userBuffer);


  for(uint8_t i = 0; i < userBuffer; i++)
  {
    Serial.printf("Red: %d \t IR: %d\n", *(redSampleBuffer + i), *(irSampleBuffer + i));
    // char PPG_message[32];
    // sprintf(PPG_message, "%d,%d", *(redSampleBuffer + i), *(irSampleBuffer + i));
    // node1BLE.BLEsendValue(PPG_message); 
  }
  
  // Serial.println("....................");
  // Serial.printf("Number of samples: %d\n", userBuffer);

// ------------ VOC Sensor ------------

  // double vocOut = 0, noxOut = 0;

  // VOCSensor.readSample(vocOut, noxOut);

  // Serial.printf("VOC: %f \t NOX: %f\n", vocOut, noxOut);

// ------------ Accelerometer ------------

  // double xAccel[32], yAccel[32], zAccel[32];
  // uint8_t numSamples = 0;

  // AccelSensor.readAcceleration(xAccel, yAccel, zAccel, &numSamples);

  // for(uint8_t i = 0; i < numSamples; i++)
  // {
  //   Serial.printf("X: %f \t Y: %f \t Z: %f\n", xAccel[i], yAccel[i], zAccel[i]);
  // }
  // Serial.println("....................");


// ------------------ ECG Sensor ------------------

  // ECGSensor.readConfigReg1();
  // delay(10);

  // delay(1);

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
