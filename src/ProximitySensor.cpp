#include <ProximitySensor.h>

ProximitySensor::ProximitySensor(int sensorPin) : sensorPin(sensorPin) {}

void ProximitySensor::init()
{
    pinMode(sensorPin, INPUT);
}

bool ProximitySensor::checkWall()
{
    if (digitalRead(sensorPin) == HIGH)
    {
        return true;
    }
    else
    {
        return false;
    }
}