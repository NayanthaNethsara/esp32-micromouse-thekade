#ifndef TIME_OF_FLIGHT_SENSOR_H
#define TIME_OF_FLIGHT_SENSOR_H

#include <Arduino.h>
#include <Adafruit_VL53L0X.h>

class TimeOfFlightSensor
{
public:
    // Constructor
    TimeOfFlightSensor();

    // Initialize the sensors
    void init();

    // Get distance from the specified sensor (0 for sensor 1, 1 for sensor 2)
    int getDistance(uint8_t sensorId);

private:
    Adafruit_VL53L0X lox1;
    Adafruit_VL53L0X lox2;

    const int cellSize = 10;

    // Helper function to select TCA9548A channel
    void tcaSelect(uint8_t i);
};

#endif