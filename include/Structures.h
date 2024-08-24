struct Coordinates
{
    int x;
    int y;
};

struct Car
{
    int speed;
    int direction;
    int angle;
    Coordinates currentCoordinate;
    float prevTime;
    float currentTime;
    float correctionFactor;
};

struct Cell
{
    bool north;
    bool east;
    bool south;
    bool west;
};
