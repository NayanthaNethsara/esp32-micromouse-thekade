#include "TimeOfFlightSensor.h"

// TCA9548A I2C address (default is 0x70)
#define TCAADDR 0x70

// Constructor
TimeOfFlightSensor::TimeOfFlightSensor() {}

// TCA9548A channel select function
void TimeOfFlightSensor::tcaSelect(uint8_t i)
{
    if (i > 7)
        return;
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}

// Initialize the sensors
void TimeOfFlightSensor::init()
{
    // Initialize I2C
    Wire.begin();

    // Initialize sensor 1 on channel 0
    tcaSelect(0);
    if (!lox1.begin())
    {
        Serial.println(F("Failed to boot VL53L0X #1"));
        while (1)
            ;
    }

    // Initialize sensor 2 on channel 1
    tcaSelect(1);
    if (!lox2.begin())
    {
        Serial.println(F("Failed to boot VL53L0X #2"));
        while (1)
            ;
    }
}

// Get distance from the specified sensor (0 for sensor 1, 1 for sensor 2)
int TimeOfFlightSensor::getDistance(uint8_t sensorId)
{
    VL53L0X_RangingMeasurementData_t measure;

    // Select the appropriate sensor
    if (sensorId == 0)
    {
        tcaSelect(0);
        lox1.rangingTest(&measure, false);
    }
    else if (sensorId == 1)
    {
        tcaSelect(1);
        lox2.rangingTest(&measure, false);
    }
    else
    {
        Serial.println("Invalid sensor ID");
        return -1; // Invalid sensor ID
    }

    // Check if the measurement is valid
    if (measure.RangeStatus != 4)
    {
        return measure.RangeMilliMeter;
    }
    else
    {
        Serial.print("Sensor ");
        Serial.print(sensorId + 1);
        Serial.println(": Out of range");
        return -1; // Out of range
    }
}