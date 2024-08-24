#ifndef CARCONTROLLER_H
#define CARCONTROLLER_H

#include <Arduino.h>
#include "UltrasonicSensor.h"
#include "GyroscopeSensor.h"
#include "TimeOfFlightSensor.h"

class CarController
{
private:
    int in1 = 33; // right motor
    int in2 = 32;
    int in3 = 26; // left motor
    int in4 = 27;
    int enA = 14; // right motor
    int enB = 25; // left motor
    int baseSpeed;
    int correctionFactor;
    int frontThreshold = 100;
    int leftThreshold = 100;
    int rightThreshold = 100;
    int backThreshold = 100;
    UltrasonicSensor leftUltrasonic{5, 18};
    UltrasonicSensor rightUltrasonic{19, 23};
    GyroscopeSensor gyroscope;
    TimeOfFlightSensor timeOfFlight;

public:
    // Constructor
    CarController(int speed, int correctionFactor);

    // Initialize the motor pins and sensors
    void init();

    // Function to move forward with balancing
    void forward(int leftDistance, int rightDistance);

    void moveForward();
    // Function to test forward movement
    void forwardTest(int speed);

    // Function to turn left by a specific angle
    void turnLeft(int angle);

    // Function to turn right by a specific angle
    void turnRight(int angle);

    // Function to print Time-of-Flight sensor values
    void printTimeOfFlightValues();
    
    // Function to stop the car
    void stop();

    // Functions to identify walls
    bool wallFront();
    bool wallLeft();
    bool wallRight();
    bool wallBack();

    // Function to get the current cell in the maze
    int getCurrentCell();

    // Functions to move the car in the maze
    void moveNorth(int direction);
    void moveEast(int direction);
    void moveSouth(int direction);
    void moveWest(int direction);
};

#endif // CARCONTROLLER_H