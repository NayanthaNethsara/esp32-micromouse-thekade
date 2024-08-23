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
            Controller.moveNorth(TheKade.direction);
            break;
        case 1:
            Controller.moveEast(TheKade.direction);
            break;
        case 2:
            Controller.moveSouth(TheKade.direction);
            break;
        case 3:
            Controller.moveWest(TheKade.direction);
            break;
        default:
            break;
        }
    }
}

void initCells(Cell cells[16][16])
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            cells[i][j] = {false, false, false, false};
        }
    }
}

int checkAdjacent(Coordinates currentPosition, Cell wall, int direction)
{
    Coordinates north = {currentPosition.x - 1, currentPosition.y};
    Coordinates east = {currentPosition.x, currentPosition.y + 1};
    Coordinates south = {currentPosition.x + 1, currentPosition.y};
    Coordinates west = {currentPosition.x, currentPosition.y - 1};

    int currentFlood = flood[currentPosition.x][currentPosition.y];

    int northFlood = (north.x >= 0) ? flood[north.x][north.y] : INT_MAX;
    int eastFlood = (east.y < 16) ? flood[east.x][east.y] : INT_MAX;
    int southFlood = (south.x < 16) ? flood[south.x][south.y] : INT_MAX;
    int westFlood = (west.y >= 0) ? flood[west.x][west.y] : INT_MAX;

    int bestDirection = -1;
    int minFlood = currentFlood;

    // Array of directions in clockwise order starting from the current direction
    int directions[4] = {direction, (direction + 1) % 4, (direction + 2) % 4, (direction + 3) % 4};

    for (int i = 0; i < 4; i++)
    {
        int currentDirection = directions[i];
        switch (currentDirection)
        {
        case 0:
            if (!wall.north && northFlood < minFlood)
            {
                bestDirection = 0;
                minFlood = northFlood;
            }
            break;
        case 1:
            if (!wall.east && eastFlood < minFlood)
            {
                bestDirection = 1;
                minFlood = eastFlood;
            }
            break;
        case 2:
            if (!wall.south && southFlood < minFlood)
            {
                bestDirection = 2;
                minFlood = southFlood;
            }
            break;
        case 3:
            if (!wall.west && westFlood < minFlood)
            {
                bestDirection = 3;
                minFlood = westFlood;
            }
            break;
        }
    }

    return bestDirection;
}

Cell walls(Coordinates currentPosition)
{
    Cell wall;

    // Check walls based on the current facing direction
    switch (TheKade.direction)
    {
    case 0: // Facing North
        wall.north = Controller.wallFront();
        wall.east = Controller.wallRight();
        wall.west = Controller.wallLeft();
        wall.south = Controller.wallBack();
        break;
    case 1: // Facing East
        wall.east = Controller.wallFront();
        wall.south = Controller.wallRight();
        wall.north = Controller.wallLeft();
        wall.west = Controller.wallBack();
        break;
    case 2: // Facing South
        wall.south = Controller.wallFront();
        wall.west = Controller.wallRight();
        wall.east = Controller.wallLeft();
        wall.north = Controller.wallBack();
        break;
    case 3: // Facing West
        wall.west = Controller.wallFront();
        wall.north = Controller.wallRight();
        wall.south = Controller.wallLeft();
        wall.east = Controller.wallBack();
        break;
    }

    cells[currentPosition.x][currentPosition.y] = wall;

    return wall;
}
