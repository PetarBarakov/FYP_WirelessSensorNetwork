#ifndef LIS2DE12_H
#define LIS2DE12_H

#include <Arduino.h>
#include <Wire.h>
#include "SensorClass.h"

#define LIS2DE12_CTRL_REG1 0x20         // <-- power/sample mode
// #define LIS2DE12_CTRL_REG2 0x21      // <-- filter
// #define LIS2DE12_CTRL_REG3 0x22      // <-- interupt
#define LIS2DE12_CTRL_REG4 0x23         // <-- scale selection
#define LIS2DE12_CTRL_REG5 0x24         // <-- FIFO selection
// #define LIS2DE12_CTRL_REG6 0x25      // <-- interrupt
// #define LIS2DE12_REFERENCE 0x26      // <-- reference for interrupt
// #define LIS2DE12_STATUS_REG 0x27     // <-- status register
#define LIS2DE12_FIFO_READ_START 0x28   // <-- acceleration data from FIFO
#define LIS2DE12_OUT_X 0x29             // <-- acceleration X-axis NORMAL MODE 
#define LIS2DE12_OUT_Y 0x2B             // <-- acceleration Y-axis NORMAL MODE
#define LIS2DE12_OUT_Z 0x2D             // <-- acceleration Z-axis NORMAL MODE
#define LIS2DE12_FIFO_CTRL_REG 0x2E     // <-- FIFO control
#define LIS2DE12_FIFO_SRC_REG 0x2F      // <-- FIFO status
// #define LIS2DE12_ACT_THS 0x3E        // <-- activity threshold
// #define LIS2DE12_ACT_DUR 0x3F        // <-- activity duration

class LIS2DE12 : public Sensor{

};

#endif