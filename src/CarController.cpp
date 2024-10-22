#include "CarController.h"

// Constructor
CarController::CarController(int speed, int correctionFactor)
{
    baseSpeed = speed;
    this->correctionFactor = correctionFactor;
}

// Initialize the motor pins and sensors
void CarController::init()
{
    pinMode(in1, OUTPUT); // right motor
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT); // left motor
    pinMode(in4, OUTPUT);
    pinMode(enA, OUTPUT); // right motor
    pinMode(enB, OUTPUT); // left motor

    // Initialize the gyroscope
    gyroscope.init();
    leftUltrasonic.init();
    rightUltrasonic.init();
    timeOfFlight.init();
}
void CarController::moveForward()
{
    // Get distance readings
    int leftDistance = leftUltrasonic.getDistance();   // Distance from the left wall
    int rightDistance = rightUltrasonic.getDistance(); // Distance from the right wall

    // Desired distance from each wall is 2cm
    int targetDistance = 2;
    int tolerance = 1; // Small tolerance

    // Base speeds for motors
    int baseSpeed = 150; // Adjust this value based on your motor specs
    float rightSpeedRatio = 10.0;
    float leftSpeedRatio = 8.7;

    // Calculate adjusted speeds
    int leftSpeed = baseSpeed * (leftSpeedRatio / 10.0);
    int rightSpeed = baseSpeed * (rightSpeedRatio / 10.0);

    // Adjust speed based on the distance from the walls
    if (leftDistance < rightDistance - tolerance)
    {
        // Too close to the left wall, increase right motor speed slightly
        rightSpeed = baseSpeed * (rightSpeedRatio / 10.0) + 15;
    }
    else if (leftDistance > rightDistance + tolerance)
    {
        // Too close to the right wall, increase left motor speed slightly
        leftSpeed = baseSpeed * (leftSpeedRatio / 10.0) + 15;
    }

    Serial.print("Left Distance: ");
    Serial.println(leftDistance);
    Serial.print("Right Distance: ");
    Serial.println(rightDistance);
    Serial.print("Left Speed: ");
    Serial.println(leftSpeed);
    Serial.print("Right Speed: ");
    Serial.println(rightSpeed);

    // Move the car forward with the adjusted speeds
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    analogWrite(enA, rightSpeed);
    analogWrite(enB, leftSpeed);

    // Small delay to allow sensor readings to update
    delay(10);
}

// Function to move forward with balancing
void CarController::forward()
{

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    analogWrite(enA, 100);
    analogWrite(enB, 87);

    delay(1000);
}

// Function to test forward movement
void CarController::forwardTest(int speed)
{
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    analogWrite(enA, 60);
    analogWrite(enB, 60);
}

// Function to turn left by a specific angle
void CarController::turnLeft(float targetAngle)
{
    float prevTime = millis();
    float currentAngle = 0;

    // Keep turning until the car reaches the target angle
    while (abs(currentAngle) < abs(targetAngle))
    {
        unsigned long currentTime = millis();

        // Get the current angle from the gyroscope
        currentAngle = gyroscope.getAngle(currentTime, &prevTime, currentAngle);

        // Adjust motor speeds to turn left in place
        int leftSpeed = baseSpeed;
        int rightSpeed = baseSpeed;

        // To turn left on the spot, the right motor runs forward, and the left motor runs backward
        leftSpeed = baseSpeed;
        rightSpeed = baseSpeed;

        // Serial.print("Left Speed: ");
        // Serial.println(leftSpeed);
        // Serial.print("Right Speed: ");
        // Serial.println(rightSpeed);

        // Execute the turn
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH); // Left motor backward
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW); // Right motor forward

        analogWrite(enA, rightSpeed);
        analogWrite(enB, leftSpeed);

        Serial.print("Current Angle: ");
        Serial.println(currentAngle);

        // Small delay to allow sensor readings to update
    }

    // Stop the car after reaching the target angle
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

// Function to turn right by a specific angle
void CarController::turnRight(float targetAngle)
{
    float prevTime = millis();
    float currentAngle = 0;

    // Keep turning until the car reaches the target angle
    while (currentAngle < targetAngle)
    {
        unsigned long currentTime = millis();

        // Get the current angle from the gyroscope
        currentAngle = gyroscope.getAngle(currentTime, &prevTime, currentAngle);

        // Adjust motor speeds to turn right in place
        int leftSpeed = baseSpeed;
        int rightSpeed = baseSpeed;

        // To turn right on the spot, the left motor runs forward, and the right motor runs backward
        leftSpeed = baseSpeed;
        rightSpeed = baseSpeed;

        // Serial.print("Left Speed: ");
        // Serial.println(leftSpeed);
        // Serial.print("Right Speed: ");
        // Serial.println(rightSpeed);

        // Execute the turn
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW); // Left motor forward
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH); // Right motor backward

        analogWrite(enA, rightSpeed);
        analogWrite(enB, leftSpeed);

        Serial.print("Current Angle: ");
        Serial.println(currentAngle);
    }

    // Stop the car after reaching the target angle
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

// Function to stop the car
void CarController::stop()
{
    analogWrite(enA, 0);
    analogWrite(enB, 0);
}

// Function to identify if there is a wall in front
bool CarController::wallFront()
{
    int distance = timeOfFlight.getDistance(0); // Use sensor 0 (front)
    return distance < frontThreshold;           // Adjust threshold as needed
}

// Function to identify if there is a wall on the left
bool CarController::wallLeft()
{
    int distance = leftUltrasonic.getDistance();
    return distance < leftThreshold; // Adjust threshold as needed
}

// Function to identify if there is a wall on the right
bool CarController::wallRight()
{
    int distance = rightUltrasonic.getDistance();
    return distance < rightThreshold; // Adjust threshold as needed
}

// Function to identify if there is a wall at the back
bool CarController::wallBack()
{
    int distance = timeOfFlight.getDistance(1); // Use sensor 1 (back)
    return distance < backThreshold;            // Adjust threshold as needed
}

// Function to calculate the current cell in the maze
int CarController::getCurrentCell()
{
    int frontDistance = timeOfFlight.getDistance(0); // Front sensor
    int backDistance = timeOfFlight.getDistance(1);  // Back sensor

    // Example logic to determine the current cell based on distances
    if (frontDistance < 100 && backDistance < 100)
    {
        return 1; // Example: Current cell ID 1
    }
    else if (frontDistance < 100)
    {
        return 2; // Example: Current cell ID 2
    }
    else if (backDistance < 100)
    {
        return 3; // Example: Current cell ID 3
    }
    else
    {
        return 0; // Default cell (no walls nearby)
    }
}

void CarController::printTimeOfFlightValues()
{
    int frontDistance = timeOfFlight.getDistance(0); // Front sensor
    int backDistance = timeOfFlight.getDistance(1);  // Back sensor

    Serial.print("Front Sensor Distance: ");
    Serial.print(frontDistance / 10);
    Serial.println(" cm");

    Serial.print("Back Sensor Distance: ");
    Serial.print(backDistance / 10);
    Serial.println(" cm");
}

// Functions to move the car in the maze
void CarController::moveNorth(int *direction, int *currentCoordinateX, int *currentCoordinateY)
{
    if (*direction == 0)
    {
        forward();
    }
    else if (*direction == 1)
    {
        turnLeft(65);
        forward();
    }
    else if (*direction == 2)
    {
        turnLeft(65);
        turnLeft(65);
        forward();
    }
    else if (*direction == 3)
    {
        turnRight(65);
        forward();
    }
    *currentCoordinateX = *currentCoordinateX - 1;
    *direction = 0;
}

void CarController::moveEast(int *direction, int *currentCoordinateX, int *currentCoordinateY)
{
    if (*direction == 0)
    {
        turnRight(65);
        forward();
    }
    else if (*direction == 1)
    {
        forward();
    }
    else if (*direction == 2)
    {
        turnLeft(65);
        forward();
    }
    else if (*direction == 3)
    {
        turnLeft(65);
        turnLeft(65);
        forward();
    }
    *currentCoordinateY = *currentCoordinateY + 1;
    *direction = 1;
}

void CarController::moveSouth(int *direction, int *currentCoordinateX, int *currentCoordinateY)
{
    if (*direction == 0)
    {
        turnLeft(65);
        turnLeft(65);
        forward();
    }
    else if (*direction == 1)
    {
        turnRight(65);
        forward();
    }
    else if (*direction == 2)
    {
        forward();
    }
    else if (*direction == 3)
    {
        turnLeft(65);
        forward();
    }
    *currentCoordinateX = *currentCoordinateX + 1;
    *direction = 2;
}

void CarController::moveWest(int *direction, int *currentCoordinateX, int *currentCoordinateY)
{

    if (*direction == 0)
    {
        turnLeft(65);
        forward();
    }
    else if (*direction == 1)
    {
        turnLeft(65);
        turnLeft(65);
        forward();
    }
    else if (*direction == 2)
    {
        turnRight(65);
        forward();
    }
    else if (*direction == 3)
    {
        forward();
    }
    *currentCoordinateY = *currentCoordinateY - 1;
    *direction = 3;
}