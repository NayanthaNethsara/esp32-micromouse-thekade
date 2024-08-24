#include <Arduino.h>
#include <Wire.h>
#include "UltrasonicSensor.h"
#include "GyroscopeSensor.h"
#include "CarController.h"
#include "TimeOfFlightSensor.h"
#include <Structures.h>
#include <climits>
#include <queue>
#include <vector>

using namespace std;

Car TheKade = {150, 0, 0, {15, 0}, 0, 0, 1.0};

GyroscopeSensor Gyroscope;
TimeOfFlightSensor TimeOfFlight;
CarController Controller(TheKade.speed, TheKade.correctionFactor); // Assuming a base speed of 150 and Kp gain of 1.0

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
    Serial.begin(115200);
    Wire.begin();
    Controller.init();
    TheKade.prevTime = millis();
}

void loop()
{

    // Controller.wallLeft();
    // Controller.wallRight();
    // Controller.forwardTest(TheKade.speed);

    // Coordinates currentCell = Controller.getCurrentCell();
    // TheKade.position = currentCell;

    // Serial.print("Current Cell: (");
    // Serial.print(currentCell.x);
    // Serial.print(", ");
    // Serial.print(currentCell.y);
    // Serial.println(")");
    Controller.printTimeOfFlightValues();
    delay(1000);
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

void initFloodFill()
{
    // Initialize all flood values to a very high value (effectively infinity)
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            flood[i][j] = INT_MAX;
        }
    }

    queue<Coordinates> q;
    vector<pair<int, int>> centerPoints = {{7, 7}, {7, 8}, {8, 7}, {8, 8}};

    // Initialize the center points with a distance of 0
    for (auto &point : centerPoints)
    {
        flood[point.first][point.second] = 0;
        q.push({point.first, point.second});
    }

    // Directions corresponding to {north, east, south, west}
    int dx[4] = {-1, 0, 1, 0};
    int dy[4] = {0, 1, 0, -1};

    while (!q.empty())
    {
        int x = q.front().x;
        int y = q.front().y;
        int distance = flood[x][y];
        q.pop();

        for (int i = 0; i < 4; i++)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];

            // Check if the new coordinates are within bounds
            if (nx >= 0 && nx < 16 && ny >= 0 && ny < 16)
            {
                bool canMove = false;

                // Check for walls in the current cell for the intended direction
                if (i == 0 && !cells[x][y].north && !cells[nx][ny].south)
                    canMove = true; // Moving north
                if (i == 1 && !cells[x][y].east && !cells[nx][ny].west)
                    canMove = true; // Moving east
                if (i == 2 && !cells[x][y].south && !cells[nx][ny].north)
                    canMove = true; // Moving south
                if (i == 3 && !cells[x][y].west && !cells[nx][ny].east)
                    canMove = true; // Moving west

                if (canMove && flood[nx][ny] > distance + 1)
                {
                    flood[nx][ny] = distance + 1;
                    q.push({nx, ny});
                }
            }
        }
    }
}
