#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(int trigPin, int echoPin)
    : trigPin(trigPin), echoPin(echoPin) {}

void UltrasonicSensor::init()
{
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

int UltrasonicSensor::getDistance()
{
    const int numReadings = 5;
    long totalDistance = 0;

    for (int i = 0; i < numReadings; i++)
    {
        // Send a 10us pulse to trigger
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        // Measure the duration of the echo pulse
        long duration = pulseIn(echoPin, HIGH, 20000L); // Timeout after 20ms
        long distance = duration * 0.034 / 2;           // Convert duration to distance in cm
        totalDistance += distance;

        delay(50); // Delay between readings
    }

    return totalDistance / numReadings;
}
