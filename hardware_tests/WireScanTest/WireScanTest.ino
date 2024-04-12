// #include "Wire.h"


// #define pinSDA 6 
// #define pinSCL 7 

// #define LED1 4

// void setup() {
//   Serial.begin(115200);

//   Wire.setPins(pinSDA, pinSCL);
//   Wire.begin();

//   pinMode(LED1, OUTPUT);
//   digitalWrite(LED1, HIGH);

//   Serial.println("Setup Done ....");

// }

// void loop() {
//   byte error, address;
//   int nDevices = 0;

//   digitalWrite(LED1, HIGH);
//   delay(1000);
//   digitalWrite(LED1, LOW);
//   delay(4000);

//   Serial.println("Scanning for I2C devices ...");
//   for(address = 0x01; address < 0x7f; address++){
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();
//     if (error == 0){
//       Serial.printf("I2C device found at address 0x%02X\n", address);
//       nDevices++;
//     } else if(error != 2){
//       Serial.printf("Error %d at address 0x%02X\n", error, address);
//     }
//   }
//   if (nDevices == 0){
//     Serial.println("No I2C devices found");
//   }
// }

#define LED1 4
#define LED2 5

void setup()
{
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop()
{
  Serial.println("KUR");
  delay(500);
  digitalWrite(LED1, HIGH);
  delay(1000);
  digitalWrite(LED1, LOW);
  delay(1000);
  digitalWrite(LED2, HIGH);
  delay(1000);
  digitalWrite(LED2, LOW);
  delay(1000);
}
