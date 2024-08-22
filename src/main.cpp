#include <Arduino.h>
#include <Wire.h>
#include "UltrasonicSensor.h"
#include "GyroscopeSensor.h"
#include "CarController.h"
#include <Structures.h>

Car TheKade = {150, 0, 0, {0, 0}, 0, 0, 1.0};

UltrasonicSensor leftUltrasonic(5, 6);
UltrasonicSensor rightUltrasonic(10, 11);

GyroscopeSensor Gyroscope;
CarController Controller(TheKade); // Assuming a base speed of 150 and Kp gain of 1.0

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    Gyroscope.init();
    leftUltrasonic.init();
    rightUltrasonic.init();
    Controller.init();
    TheKade.prevTime = millis();
}

void loop()
{
    // Get current time
    TheKade.currentTime = millis();

    // Calculate the angle using the gyroscope
    // angle = gyro.getAngle(currentTime, &prevTime, angle);

    // // Print the current angle
    // Serial.print("Angle: ");
    // Serial.println(angle);

    Serial.print("Right Distance: ");
    Serial.println(rightUltrasonic.getDistance());

    Serial.print("Left Distance: ");
    Serial.println(leftUltrasonic.getDistance());

    delay(1000);

    // car.forward(leftUltrasonic.getDistance(), rightUltrasonic.getDistance());
}
