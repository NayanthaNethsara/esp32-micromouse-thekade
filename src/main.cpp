#include <Arduino.h>
#include <Wire.h>
#include "UltrasonicSensor.h"
#include "GyroscopeSensor.h"
#include "CarController.h"
#include <Structures.h>
#include <climits>
#include <queue>

Car TheKade = {150, 0, 0, {15, 0}, 0, 0, 1.0};

UltrasonicSensor leftUltrasonic(5, 6);
UltrasonicSensor rightUltrasonic(10, 11);

GyroscopeSensor Gyroscope;
CarController Controller(TheKade); // Assuming a base speed of 150 and Kp gain of 1.0

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};
// Initialize a 2D array of Cell objects
Cell cells[16][16];
int flood[16][16];

void initCells(Cell cells[16][16]);
void initFloodFill();
Cell walls(Coordinates currentPosition);
int checkAdjacent(Coordinates currentPosition, Cell wall, int direction);

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
    initCells(cells);
    initFloodFill();
    int currentFlood = flood[TheKade.currentCoordinate.x][TheKade.currentCoordinate.y];
    while (currentFlood != 0)
    {
        currentFlood = flood[TheKade.currentCoordinate.x][TheKade.currentCoordinate.y];
        Cell wall = walls(TheKade.currentCoordinate);

        int bestDirection = checkAdjacent(TheKade.currentCoordinate, wall, TheKade.direction);

        if (bestDirection == -1)
        {
            initFloodFill();
            bestDirection = checkAdjacent(TheKade.currentCoordinate, wall, TheKade.direction);
        }

        switch (bestDirection)
        {
        case 0:
            CarController.moveNorth();
            break;
        case 1:
            CarController.moveEast();
            break;
        case 2:
            CarController.moveSouth();
            break;
        case 3:
            CarController.moveWest();
            break;
        default:
            break;
        }
    }
}
