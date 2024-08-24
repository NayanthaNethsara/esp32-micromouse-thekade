#include "GyroscopeSensor.h"
#include "SerialMultiplexer.h"

void GyroscopeSensor::init()
{
    i2cMultiplexer.tcaSelect(2); // Select the channel connected to the gyroscope

    // Set CTRL_REG1 to configure the gyroscope
    Wire.beginTransmission(L3G4200D_ADDRESS);
    Wire.write(L3G4200D_CTRL_REG1);
    Wire.write(0x0F); // Normal power mode, all axes enabled, 100 Hz
    Wire.endTransmission();
}

void GyroscopeSensor::calibrateGyro()
{
    i2cMultiplexer.tcaSelect(2); // Select the channel connected to the gyroscope

    const int numSamples = 1000;
    long totalZ = 0;

    for (int i = 0; i < numSamples; i++)
    {
        totalZ += readGyroZ(); // Accumulate Z-axis readings
        delay(1);              // Small delay between readings
    }

    zOffset = totalZ / numSamples; // Calculate average offset
}

float GyroscopeSensor::readGyroZ()
{
    i2cMultiplexer.tcaSelect(2); // Select the channel connected to the gyroscope

    // Existing code to read the Z-axis rotation rate
    Wire.beginTransmission(L3G4200D_ADDRESS);
    Wire.write(L3G4200D_OUT_Z_L | 0x80); // Set auto-increment bit
    Wire.endTransmission();

    Wire.requestFrom(L3G4200D_ADDRESS, 2);
    while (Wire.available() < 2)
        ;

    uint8_t zL = Wire.read();
    uint8_t zH = Wire.read();

    int16_t z = (zH << 8) | zL;

    // Convert to degrees per second and subtract the offset
    float zRotationRate = (z * 0.00875); // Adjust for offset

    if (abs(zRotationRate) < 0.1)
    {
        zRotationRate = 0;
    }

    return zRotationRate;
}

float GyroscopeSensor::getAngle(float currentTime, float *prevTime, float currentAngle)
{
    i2cMultiplexer.tcaSelect(2); // Select the channel connected to the gyroscope

    float deltaTime = (currentTime - *prevTime) / 1000.0;

    static float angle = currentAngle;
    *prevTime = millis();

    // Read gyroscope data
    float zRotationRate = readGyroZ();

    // Integrate the zRotationRate to get the angle
    angle += zRotationRate * deltaTime;

    // Keep angle in the range of -180 to 180 degrees
    if (angle > 180)
    {
        angle -= 360;
    }
    else if (angle < -180)
    {
        angle += 360;
    }

    // If the robot is moving forward and no significant rotation is detected,
    // reset the angle to zero to prevent drift accumulation.
    if (fabs(zRotationRate) < 0.1)
    {
        angle = 0;
    }

    return angle;
}
