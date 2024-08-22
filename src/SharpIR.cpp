#include "SharpIR.h"

SharpIR::SharpIR(int analogPin) : analogPin(analogPin) {}

void SharpIR::init()
{
    pinMode(analogPin, OUTPUT);
}

int SharpIR::getDistance()
{
    int sensorValue = analogRead(analogPin);        // Read the analog value from the sensor
    float distance = analogToDistance(sensorValue); // Convert analog value to distance
    return distance;
}

// Function to convert the analog value to distance in cm
float SharpIR::analogToDistance(int analogValue)
{
    if (analogValue < 11)
    {              // Prevent division by zero or negative values
        return -1; // Invalid value
    }
    return 2076.0 / (analogValue - 11.0);
}
