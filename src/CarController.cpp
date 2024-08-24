#include "CarController.h"

// Constructor
CarController::CarController(int speed, int correctionFactor)
{
    baseSpeed = speed;
    this->correctionFactor = correctionFactor;
}

// Initialize the motor pins
void CarController::init()
{
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    leftUltrasonic.init();
    rightUltrasonic.init();
}

// Function to move forward with balancing
void CarController::forward(int leftDistance, int rightDistance)
{
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

// Function to test forward movement
void CarController::forwardTest(int speed)
{
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    analogWrite(enA, speed);
    analogWrite(enB, speed);
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

// Functions to identify walls (Stub - Add actual sensor logic)
bool CarController::wallFront()
{
    // Implement wall detection logic
    return false;
}

bool CarController::wallLeft()
{
    int Distance = leftUltrasonic.getDistance();

    if (Distance < 10)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CarController::wallRight()
{
    // Implement wall detection logic
    return false;
}

bool CarController::wallBack()
{
    // Implement wall detection logic
    return false;
}

// Functions to move the car in the maze
void CarController::moveNorth(int direction)
{
    if (direction == 0)
    {
        forwardTest(baseSpeed); // Move forward
    }
    else if (direction == 1)
    {
        turnLeft(90);
        forwardTest(baseSpeed);
    }
    else if (direction == 2)
    {
        turnLeft(180);
        forwardTest(baseSpeed);
    }
    else if (direction == 3)
    {
        turnRight(90);
        forwardTest(baseSpeed);
    }
}

void CarController::moveEast(int direction)
{
    if (direction == 0)
    {
        turnRight(90);
        forwardTest(baseSpeed);
    }
    else if (direction == 1)
    {
        forwardTest(baseSpeed);
    }
    else if (direction == 2)
    {
        turnLeft(90);
        forwardTest(baseSpeed);
    }
    else if (direction == 3)
    {
        turnLeft(180);
        forwardTest(baseSpeed);
    }
}

void CarController::moveSouth(int direction)
{
    if (direction == 0)
    {
        turnLeft(180);
        forwardTest(baseSpeed);
    }
    else if (direction == 1)
    {
        turnRight(90);
        forwardTest(baseSpeed);
    }
    else if (direction == 2)
    {
        forwardTest(baseSpeed);
    }
    else if (direction == 3)
    {
        turnLeft(90);
        forwardTest(baseSpeed);
    }
}

void CarController::moveWest(int direction)
{
    if (direction == 0)
    {
        turnLeft(90);
        forwardTest(baseSpeed);
    }
    else if (direction == 1)
    {
        turnLeft(180);
        forwardTest(baseSpeed);
    }
    else if (direction == 2)
    {
        turnRight(90);
        forwardTest(baseSpeed);
    }
    else if (direction == 3)
    {
        forwardTest(baseSpeed);
    }
}
