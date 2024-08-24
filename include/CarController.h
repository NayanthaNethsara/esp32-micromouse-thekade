#ifndef CARCONTROLLER_H
#define CARCONTROLLER_H

#include <Arduino.h>
#include "ultrasonicSensor.h"

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
    UltrasonicSensor leftUltrasonic{5, 18};
    UltrasonicSensor rightUltrasonic{19, 23};

public:
    // Constructor
    CarController(int speed, int correctionFactor);

    // initialize the motor pins
    void init();

    // Function to move forward with balancing
    void forward(int leftDistance, int rightDistance);

    // Function to test forward movement
    void forwardTest(int speed);

    // Function to turn left by a specific angle
    void turnLeft(int angle);

    // Function to turn right by a specific angle
    void turnRight(int angle);

    // Function to stop the car
    void stop();

    // Functions to identify walls
    bool wallFront();
    bool wallLeft();
    bool wallRight();
    bool wallBack();

    // Functions to move the car in the maze
    void moveNorth(int direction);
    void moveEast(int direction);
    void moveSouth(int direction);
    void moveWest(int direction);
};

#endif // CARCONTROLLER_H
