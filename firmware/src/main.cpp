#include "header.h"


// I2C GPIO pins
#define pinSDA 6 
#define pinSCL 7 

#ifdef ESP32_SENSORS_MEASUREMENT

// #define PROGRAM_TRH_SENSOR
#define PROGRAM_PPG_SENSOR
// #define PROGRAM_VOC_SENSOR
// #define PROGRAM_ACC_ECG_SENSOR

// ============= END OF PROGRAMING CONFIGURATION =============

#define SHT40_ADDRESS 0x44
#define MAX30102_ADDRESS 0x57
#define SGP41_ADDRESS 0x59
#define LIS2DE12_ADDRESS 0x18

#define ECG_SPI_MISO 0
#define ECG_SPI_MOSI 3
#define ECG_SPI_SCK 1
#define ECG_SPI_CS 10

//Initialise an object for the BLE communication
#define TH_UUID "e87917aa-103e-4d61-a11d-ae0d09963b64"
#define PPG_UUID "4703e542-0870-4fd7-8685-0dc1b371d700"
#define VOC_UUID "8c367fd0-fd14-49a8-bf73-d5dfa76f6e1a"
#define ECG_ACC_UUID "b37a5a9f-97ac-4f36-9f1f-024c0a3e896d"

#define TH_SERVICE "e76df599-c633-455e-8c90-fab57811c73c"
#define PPG_SERVICE "781bb971-f4b1-46d9-a923-4bba480f5f11"
#define VOC_SERVICE "a4fce472-2bfa-48a5-bd49-32dde7ad80ab"
#define ECG_ACC_SERVICE "d9395096-8d5b-4a28-8474-d615bb170069"


// //Initialise an object for the PPG sensor
#ifdef PROGRAM_PPG_SENSOR
MAX30102 PPGSensor(MAX30102_ADDRESS);
publisherBLE PPGNode("FYP_PPG_Node", PPG_UUID, PPG_SERVICE);
#endif //PROGRAM_PPG_SENSOR

// //Initialise an object for the VOC sensor
#ifdef PROGRAM_VOC_SENSOR
SGP41 VOCSensor(SGP41_ADDRESS);
publisherBLE VOCNode("FYP_VOC_Node", VOC_UUID, VOC_SERVICE);
#endif //PROGRAM_VOC_SENSOR

#ifdef PROGRAM_TRH_SENSOR
SHT40 TRHSensor(SHT40_ADDRESS);
publisherBLE THNode("FYP_TH_Node", TH_UUID, TH_SERVICE);
#endif //PROGRAM_TRH_SENSOR

//Initialise Acclelerometer
#ifdef PROGRAM_ACC_ECG_SENSOR
LIS2DE12 AccelSensor(LIS2DE12_ADDRESS);
ADS1292 ECGSensor(ECG_SPI_CS);
publisherBLE ECG_ACC_Node("FYP_ECG_ACC_Node", ECG_ACC_UUID, ECG_ACC_SERVICE);
#endif //PROGRAM_ACC_ECG_SENSOR

void setup() {
  //Initialise Serial 
  Serial.begin(115200);
  //Initialise I2C communication
  Wire.setPins(pinSDA, pinSCL);
  Wire.begin();


  #ifdef PROGRAM_TRH_SENSOR
  THNode.BLEinit();
  #endif //PROGRAM_TRH_SENSOR

  //Iniitalise PPG Sensor
  #ifdef PROGRAM_PPG_SENSOR
  PPGSensor.init( (uint8_t) 16,                //sampleAverage
                  (uint8_t) 1,                //mode
                  (uint8_t) 20,                //typCurrent [mA]
                  (uint8_t) 0b10,             //SpO2ADCRange corresponding to bits
                  (uint16_t) 800,              //SpO2SampleRate
                  (uint8_t) 18                //SpO2PulseWidth
                );  

  PPGNode.BLEinit();
  #endif //PROGRAM_PPG_SENSOR

  #ifdef PROGRAM_VOC_SENSOR
  uint16_t SRAW_VOC_INTIAL = 0;
  VOCSensor.executeConditioning(SRAW_VOC_INTIAL);
  // Serial.printf("Initial SRAW VOC: %d\n", SRAW_VOC_INTIAL);
  VOCNode.BLEinit();
  #endif //PROGRAM_VOC_SENSOR

  #ifdef PROGRAM_ACC_ECG_SENSOR
  
  SPI.begin(ECG_SPI_SCK, ECG_SPI_MISO, ECG_SPI_MOSI, ECG_SPI_CS);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setFrequency(1000000);
  pinMode(ECG_SPI_CS, OUTPUT);
  digitalWrite(ECG_SPI_CS, HIGH);

  ECGSensor.init(250, 6); //Sampling Rate, Gain
  
  ECG_ACC_Node.BLEinit();

  AccelSensor.init(100, 2); //Sampler rate of 100Hz and scale of 2g


  #endif //PROGRAM_ACC_ECG_SENSOR
}

void loop() {


//------------ Temperature and Relative Humidity Sensor ------------

  #ifdef PROGRAM_TRH_SENSOR

  double temp, rh;
  static uint32_t TRHsampleTimeStamp = millis();
  
  if(millis() - TRHsampleTimeStamp >= 200)
  {
    TRHsampleTimeStamp = millis();
    TRHSensor.readTempHumid(temp, rh);


    //Transmit TRH data
    char TRHmessage[32];
    sprintf(TRHmessage, "%d,%f,%f", TRHsampleTimeStamp, temp, rh);
    THNode.BLEsendValue(TRHmessage);

    Serial.printf("TEMP: %f \t RH: %f \n", temp, rh);
  }
  #endif //PROGRAM_TRH_SENSOR
  
//------------ PPG Sensor ------------  

  #ifdef PROGRAM_PPG_SENSOR

  int32_t HR = 0, SpO2 = 0;
  static uint32_t PPGSampleTime = millis();

  //Start sampling
  PPGSensor.SpO2andHRread(&HR, &SpO2);

  if(millis() - PPGSampleTime >= 1000)
  {
    Serial.printf("Heart Rate: %d \t Sp02: %d\n", HR, SpO2);
    
    //Transmit PPG data
    char PPGmessage[32];
    sprintf(PPGmessage, "%d,%d,%d", PPGSampleTime, HR, SpO2);
    PPGNode.BLEsendValue(PPGmessage);
    
    PPGSampleTime = millis();
  }

  #endif //PROGRAM_PPG_SENSOR


// ------------ VOC Sensor ------------

  #ifdef PROGRAM_VOC_SENSOR

  uint16_t vocOut = 0, noxOut = 0;
  bool vocjump = false, noxjump = false;

  static uint32_t VOCSampleTimeStamp = millis();

  if(millis() - VOCSampleTimeStamp >= 1000)
  {
    VOCSensor.readSample(vocOut, noxOut, vocjump, noxjump);
    Serial.printf("VOC: %d \t NOX: %d\t VOC JUMP: %d \t NOX JUMP: %d\n", vocOut, noxOut, vocjump, noxjump);
    VOCSampleTimeStamp = millis();

    char VOCmessage[32];
    sprintf(VOCmessage, "%d,%d,%d,%d,%d", VOCSampleTimeStamp, vocOut, noxOut, vocjump, noxjump);
    VOCNode.BLEsendValue(VOCmessage);
  }

  #endif //PROGRAM_VOC_SENSORss

// ------------ Accelerometer ------------
  #ifdef PROGRAM_ACC_ECG_SENSOR

  double xAccel = 0, yAccel = 0, zAccel= 0;
  bool movementDetected = false;

  AccelSensor.readAcceleration(&xAccel, &yAccel, &zAccel);
  movementDetected = AccelSensor.detectMovement(xAccel, yAccel, zAccel);

  static uint32_t AccelSampleTimeStamp = millis();

  // if (millis() - AccelSampleTimeStamp >= 100)
  // {
  //   AccelSampleTimeStamp = millis();
  //   Serial.printf("X: %f \t Y: %f \t Z: %f \t Movement: %d\n", xAccel, yAccel, zAccel, movementDetected);
  // }


  // ------------------ ECG Sensor ------------------

  static uint32_t ECGsampleTimeStamp = millis();

  int32_t s32_ch1_data = ECGSensor.readRawECG();
  int16_t ecg_data_filtered = ECGSensor.filterECG(s32_ch1_data);
  uint8_t ecgHR = ECGSensor.readHR(ecg_data_filtered);
  
  if((millis() - ECGsampleTimeStamp >= 1000) || movementDetected) 
  {
    ECGsampleTimeStamp = millis();
    Serial.printf("ECG_HR: %d \t ECG_RAW: %d \t Filtered: %d \n", ecgHR, s32_ch1_data, ecg_data_filtered);
    // Serial.printf("X: %f \t Y: %f \t Z: %f \t Movement: %d\n", xAccel, yAccel, zAccel, movementDetected);

    char ECG_ACC_message[64];
    sprintf(ECG_ACC_message, "%d,%d,%f,%f,%f,%d", ECGsampleTimeStamp, ecgHR, xAccel, yAccel, zAccel, movementDetected);
    ECG_ACC_Node.BLEsendValue(ECG_ACC_message);

  }

  #endif //  #ifdef PROGRAM_ECG_SENSOR
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

  //---------------- Input Measurements ------------------
  double input_current = 0;
  double input_busVoltage = 0;
  double input_shuntVoltage = 0;

  InputCurrentSense.readBusVoltage(input_busVoltage);
  InputCurrentSense.readShuntVoltage(input_shuntVoltage);

  input_current = input_shuntVoltage / INPUT_CURRENT_SENSE_RESISTANCE;
  input_current = input_current * 1000; //Convert to mA

  // ------------------ ESP Measurements ------------------

  double esp_current = 0;
  double esp_busVoltage = 0;
  double esp_shuntVoltage = 0;

  ESPCurrentSense.readBusVoltage(esp_busVoltage);
  ESPCurrentSense.readShuntVoltage(esp_shuntVoltage);

  esp_current = esp_shuntVoltage / ESP_CURRENT_SENSE_RESISTANCE;
  esp_current = esp_current * 1000; //Convert to mA

  //----------------- Sensor Measurement ------------------
  double sensor_current = 0;
  double sensor_busVoltage = 0;
  double sensor_shuntVoltage = 0;

  SensorsCurrentSense.readBusVoltage(sensor_busVoltage);
  SensorsCurrentSense.readShuntVoltage(sensor_shuntVoltage);

  sensor_current = sensor_shuntVoltage / SENSORS_CURRENT_SENSE_RESISTANCE;
  sensor_current = sensor_current * 1000; //Convert to mA


  //----------------- PPG Measurement ------------------
  double ppg_current = 0;
  double ppg_busVoltage = 0;
  double ppg_shuntVoltage = 0;

  PPGCurrentSense.readBusVoltage(ppg_busVoltage);
  PPGCurrentSense.readShuntVoltage(ppg_shuntVoltage);

  ppg_current = ppg_shuntVoltage / PPG_CURRENT_SENSE_RESISTANCE;
  ppg_current = ppg_current * 1000; //Convert to mA

  //---------------- Print Measurements ----------------
  Serial.printf("I_in: %f , V_in: %f , ", input_current, input_busVoltage);
  Serial.printf("I_esp: %f , V_esp: %f , ", esp_current, esp_busVoltage);
  Serial.printf("I_sens: %f , V_sens: %f , ", sensor_current, sensor_busVoltage);
  Serial.printf("I_ppg: %f , V_ppg: %f\n", ppg_current, ppg_busVoltage);

  // Serial.printf("I_in: %f \t V_in: %f \t I_esp: %f \t V_esp: %f\n", input_current, input_busVoltage, esp_current, esp_busVoltage);
  delay(10);
}

#endif // ESP32_POWER_MEASUREMENT
