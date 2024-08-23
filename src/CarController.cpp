#include "CarController.h"
#include <Structures.h>

// Constructor
CarController::CarController(Car car)
{
    baseSpeed = car.speed;
    correctionFactor = car.correctionFactor;
}

// initialize the motor pins
void CarController::init()
{
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
}

// Function to move forward with balancing
void CarController::forward()
{
    int leftDistance = leftUltrasonic.getDistance();
    int rightDistance = rightUltrasonic.getDistance();
    int error = leftDistance - rightDistance;
    int speedLeft = baseSpeed - (error * correctionFactor);
    int speedRight = baseSpeed + (error * correctionFactor);

    speedLeft = constrain(speedLeft, 0, 255);
    speedRight = constrain(speedRight, 0, 255);

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    analogWrite(enA, speedLeft);
    analogWrite(enB, speedRight);
}

// Function to turn left by a specific angle
void CarController::turnLeft(int angle)
{
    int turnSpeed = map(angle, 0, 180, baseSpeed, 0);

    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH); // Reverse left motor
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW); // Forward right motor

    analogWrite(enA, turnSpeed);
    analogWrite(enB, baseSpeed);
}

// Function to turn right by a specific angle
void CarController::turnRight(int angle)
{
    int turnSpeed = map(angle, 0, 180, baseSpeed, 0);

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW); // Forward left motor
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH); // Reverse right motor

    analogWrite(enA, baseSpeed);
    analogWrite(enB, turnSpeed);
}

// Function to stop the car
void CarController::stop()
{
    analogWrite(enA, 0);
    analogWrite(enB, 0);
}

void CarController::forwardTest(int speed)
{
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    analogWrite(enA, speed);
    analogWrite(enB, speed);
}

// Functions to idencify walls
bool CarController::wallFront()
{
    return false;
}

bool CarController::wallLeft()
{
    return false;
}

bool CarController::wallRight()
{
    return false;
}

bool CarController::wallBack()
{
    return false;
}

// Functions to move the car in the maze
void CarController::moveNorth(int direction)
{
    if (direction == 0)
    {
        // moveForward();
    }
    else if (direction == 1)
    {
        // turnLeft();
        // moveForward();
    }
    else if (direction == 2)
    {
        // turnLeft();
        // turnLeft();
        // moveForward();
    }
    else if (direction == 3)
    {
        // turnRight();
        // moveForward();
    }
}

void CarController::moveEast(int direction)
{
    if (direction == 0)
    {
        // turnRight();
        // moveForward();
    }
    else if (direction == 1)
    {
        // moveForward();
    }
    else if (direction == 2)
    {
        // turnLeft();
        // moveForward();
    }
    else if (direction == 3)
    {
        // turnLeft();
        // turnLeft();
        // moveForward();
    }
}

void CarController::moveSouth(int direction)
{
    if (direction == 0)
    {
        // turnLeft();
        // turnLeft();
        // moveForward();
    }
    else if (direction == 1)
    {
        // turnRight();
        // moveForward();
    }
    else if (direction == 2)
    {
        // moveForward();
    }
    else if (direction == 3)
    {
        // turnLeft();
        // moveForward();
    }
}

void CarController::moveWest(int direction)
{
    if (direction == 0)
    {
        // turnLeft();
        // moveForward();
    }
    else if (direction == 1)
    {
        // turnLeft();
        // turnLeft();
        // moveForward();
    }
    else if (direction == 2)
    {
        // turnRight();
        // moveForward();
    }
    else if (direction == 3)
    {
        // moveForward();
    }
}