#ifndef PROXIMITY_SENSOR_H
#define PROXIMITY_SENSOR_H

#include <Arduino.h>

class ProximitySensor
{
public:
    ProximitySensor(int sensorPin);
    void init();
    bool checkWall();

private:
    int sensorPin;
};

#endif
