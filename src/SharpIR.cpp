#include "SharpIR.h"

// SharpIR::SharpIR(int analogPin) : analogPin(analogPin) {}

void SharpIR::init()
{
    pinMode(analogPin, INPUT);
}

int SharpIR::getDistance()
{
    int sensorValue = analogRead(analogPin); // Read the analog value from the sensor

    // Convert analog value to distance (in cm) using a formula derived from the sensor's datasheet
    // This formula is specific to the Sharp GP2Y0A21YK0F, which has a range of approximately 10cm to 80cm
    float voltage = sensorValue * (3.3 / 4095.0);   // Convert the analog value to voltage
    float distance = 29.988 * pow(voltage, -1.173); // Use the sensor's characteristic equation

    return static_cast<int>(distance); // Return the distance as an integer value
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
