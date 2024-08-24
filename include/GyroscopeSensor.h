#ifndef GYROSCOPE_SENSOR_H
#define GYROSCOPE_SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include "SerialMultiplexer.h"

class GyroscopeSensor
{
public:
    void init();
    void calibrateGyro();
    float getAngle(float currentTime, float *prevTime, float currentAngle);
    float readGyroZ();

private:
    // L3G4200D I2C address
    const int L3G4200D_ADDRESS = 0x69;
    // Registers
    const int L3G4200D_CTRL_REG1 = 0x20;
    const int L3G4200D_OUT_Z_L = 0x2C;
    const int L3G4200D_OUT_Z_H = 0x2D;

    SerialMultiplexer i2cMultiplexer;

    float zOffset = 0;

    // Methods
};

#endif