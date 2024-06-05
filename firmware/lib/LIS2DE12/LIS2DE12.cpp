#include "LIS2DE12.h"

LIS2DE12::LIS2DE12(uint8_t sensorAddress) : Sensor(sensorAddress) {
}

void LIS2DE12::setSampleRate(uint16_t sampleRate)
{
    uint8_t axisEN = 0b1111; // Enable all axis

    /* Sample rate set by ORD bits:
        0b0000      --> Power down mode
        0b0001      --> 1 Hz
        0b0010      --> 10 Hz
        0b0011      --> 25 Hz
        0b0100      --> 50 Hz
        0b0101      --> 100 Hz
        0b0110      --> 200 Hz
        0b0111      --> 400 Hz
        0b1000      --> 1620 Hz
        0b1001      --> 5376 Hz
    */

    uint8_t sampleRateBits = 0b0000;

    switch (sampleRate)
    {
    case 1:
        sampleRateBits = 0b0001;
        break;
    case 10:
        sampleRateBits = 0b0010;
        break;
    case 25:
        sampleRateBits = 0b0011;
        break;
    case 50:
        sampleRateBits = 0b0100;
        break;
    case 100:
        sampleRateBits = 0b0101;
        break;
    case 200:
        sampleRateBits = 0b0110;
        break;
    case 400:
        sampleRateBits = 0b0111;
        break;
    case 1620:
        sampleRateBits = 0b1000;
        break;
    case 5376:
        sampleRateBits = 0b1001;
        break;
    default:
        sampleRateBits = 0b0000;
        break;
    }

    uint8_t rxBuffer = (sampleRateBits << 4 ) | axisEN;

    writeToReg(LIS2DE12_CTRL_REG1, rxBuffer);
}

void LIS2DE12::setScale(uint8_t inputscale)
{
    scale = inputscale;

    uint8_t scaleBits = 0;

    /* Scale set by FS bits:
        0b00        --> 2g
        0b01        --> 4g
        0b10        --> 8g
        0b11        --> 16g
    */

    switch (scale)
    {
    case 2:
        scaleBits = 0b00;
        break;
    case 4:
        scaleBits = 0b01;
        break;
    case 8:
        scaleBits = 0b10;
        break;
    case 16:
        scaleBits = 0b11;
        break;
    default:
        scaleBits = 0b00;
        break;
    }

    uint8_t rxBuffer = scaleBits << 4;

    writeToReg(LIS2DE12_CTRL_REG4, rxBuffer);
}

void LIS2DE12::setFIFO()
{
    // FIFO enable bits:
    // 0b0         --> FIFO Disable
    // 0b1         --> FIFO Enable

    writeToReg(LIS2DE12_CTRL_REG5, 0x40); // 0b0100 0000
}

void LIS2DE12::init(uint16_t sampleRate, uint8_t inputScale)
{
    reset();
    setSampleRate(sampleRate);
    setScale(inputScale);
    setFIFO();
    setFIFOMode(FIFO_STREAM_MODE);
}

void LIS2DE12::setFIFOMode(uint8_t mode)
{
    // FIFO mode set by FMODE bits:
    // 0b00        --> Bypass mode
    // 0b01        --> FIFO mode
    // 0b10        --> Stream mode
    // 0b11        --> Stream-to-FIFO mode

    //Selection of Stream mode;

    writeToReg(LIS2DE12_FIFO_CTRL_REG, mode);
}

void LIS2DE12::readAcceleration(double* x, double* y, double* z)
{
    int8_t xRaw, yRaw, zRaw;
        
    uint8_t rxBuff;
    writeSensor1Byte(LIS2DE12_OUT_X);
    readSensorBytes(&rxBuff, 1);
    xRaw = (int8_t) rxBuff;

    writeSensor1Byte(LIS2DE12_OUT_Y);
    readSensorBytes(&rxBuff, 1);
    yRaw = (int8_t) rxBuff;

    writeSensor1Byte(LIS2DE12_OUT_Z);
    readSensorBytes(&rxBuff, 1);
    zRaw = (int8_t) rxBuff;

    *x = (double) xRaw * scale / 256;
    *y = (double) yRaw * scale / 256;
    *z = (double) zRaw * scale / 256;

}

bool LIS2DE12::detectMovement(double x, double y, double z)
{
    const double threshold = 1;
    const uint8_t bufferLength = 10;

    double xyz_magnitude = sqrt(x*x + y*y + z*z);

    static double xyzBuffer[bufferLength] = {0};
    double buffer_avverage = 0;

    for (uint8_t i = 0; i < bufferLength - 1; i++)
    {
        xyzBuffer[i] = xyzBuffer[i + 1];
        buffer_avverage += xyzBuffer[i];
    }

    xyzBuffer[bufferLength - 1] = xyz_magnitude;
    buffer_avverage += xyzBuffer[bufferLength - 1];
    buffer_avverage /= bufferLength;

    return (buffer_avverage > threshold);


}

void LIS2DE12::reset()
{
    writeToReg(LIS2DE12_CTRL_REG5, 0x80);
    setFIFOMode(FIFO_BYPASS_MODE);
}