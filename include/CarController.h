#ifndef CARCONTROLLER_H
#define CARCONTROLLER_H

#include <Arduino.h>

class CarController
{
private:
    int in1 = 13;
    int in2 = 12;
    int in3 = 8;
    int in4 = 7;
    int enA = 10;
    int enB = 9;
    int baseSpeed;
    int correctionFactor;

public:
    // Constructor
    CarController(Car car);

    // initialize the motor pins
    void init();

    // Function to move forward with balancing
    void forward(int leftDistance, int rightDistance);

    void forwardTest(int speed);

    // Function to turn left by a specific angle
    void turnLeft(int angle);

    // Function to turn right by a specific angle
    void turnRight(int angle);

    // Function to stop the car
    void stop();
};

#endif // CARCONTROLLER_H
