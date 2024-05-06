#ifndef I2C_TEST_H
#define I2C_TEST_H

#define pinSDA 6 
#define pinSCL 7 
#define LED1 4

void I2CSearchInit(); //Use if the I2C line has not been initialised

void I2CSearchAddr();

#endif