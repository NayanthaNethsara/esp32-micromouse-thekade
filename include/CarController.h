#ifndef CARCONTROLLER_H
#define CARCONTROLLER_H

#include <Arduino.h>

class CarController
{
private:
    int in1 = 32;
    int in2 = 33;
    int in3 = 26;
    int in4 = 27;
    int enA = 25;
    int enB = 14;
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

    void moveNorth(int direction);
    void moveEast(int direction);
    void moveSouth(int direction);
    void moveWest(int direction);

    bool wallFront();
    bool wallLeft();
    bool wallRight();
    bool wallBack();
};

#endif // CARCONTROLLER_H
