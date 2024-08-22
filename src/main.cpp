#include <Wire.h>
#include <Adafruit_VL53L0X.h>

// Create objects for the VL53L0X sensors
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

// TCA9548A I2C address (default is 0x70)
#define TCAADDR 0x70

// L3G4200D gyroscope I2C address
const int L3G4200D_ADDRESS = 0x70;

// Registers for L3G4200D
const int L3G4200D_CTRL_REG1 = 0x20;
const int L3G4200D_OUT_Z_L = 0x2C;
const int L3G4200D_OUT_Z_H = 0x2D;

// Variables for angle calculations
float zOffset = 0;
float prevTime = 0;
float currentAngle = 0; // Moved to global scope for persistence

// Function prototypes
void tcaSelect(uint8_t i);
void initGyro();
void calibrateGyro();
float readGyroZ();
float getAngle(float currentTime, float *prevTime, float currentAngle);

// TCA9548A channel select function
void tcaSelect(uint8_t i)
{
    if (i > 7)
        return;
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}

// Function to initialize the L3G4200D gyroscope
void initGyro()
{
    Wire.beginTransmission(L3G4200D_ADDRESS);
    Wire.write(L3G4200D_CTRL_REG1);
    Wire.write(0x0F); // Normal power mode, all axes enabled, 100 Hz
    Wire.endTransmission();
}

// Function to calibrate the gyroscope (average Z-axis offset)
void calibrateGyro()
{
    const int numSamples = 1000;
    long totalZ = 0;

    for (int i = 0; i < numSamples; i++)
    {
        totalZ += readGyroZ(); // Accumulate Z-axis readings
        delay(1);              // Small delay between readings
    }

    zOffset = totalZ / numSamples; // Calculate average offset
}

// Function to read the Z-axis data from the gyroscope
float readGyroZ()
{
    Wire.beginTransmission(L3G4200D_ADDRESS);
    Wire.write(L3G4200D_OUT_Z_L | 0x80); // Set auto-increment bit
    Wire.endTransmission();

    Wire.requestFrom(L3G4200D_ADDRESS, 2);
    while (Wire.available() < 2)
        ;

    uint8_t zL = Wire.read();
    uint8_t zH = Wire.read();

    int16_t z = (zH << 8) | zL;

    // Convert to degrees per second and subtract the offset
    float zRotationRate = (z * 0.00875) - zOffset; // Adjust for offset

    return zRotationRate;
}

// Function to calculate the angle based on gyroscope data
float getAngle(float currentTime, float *prevTime, float currentAngle)
{
    float deltaTime = (currentTime - *prevTime) / 1000.0;
    *prevTime = millis();

    // Read gyroscope Z-axis data
    float zRotationRate = readGyroZ();

    // Integrate the zRotationRate to get the angle
    currentAngle += zRotationRate * deltaTime;

    // Keep angle in the range of -180 to 180 degrees
    if (currentAngle > 180)
    {
        currentAngle -= 360;
    }
    else if (currentAngle < -180)
    {
        currentAngle += 360;
    }

    // // Reset the angle to zero if there's no significant rotation
    // if (fabs(zRotationRate) < 0.1)
    // {
    //     currentAngle = 0;
    // }

    return currentAngle;
}

void setup()
{
    Serial.begin(115200);
    Wire.begin(); // Initialize I2C

    // Initialize the VL53L0X sensors
    tcaSelect(0);
    if (!lox1.begin())
    {
        Serial.println(F("Failed to boot VL53L0X #1"));
        while (1)
            ;
    }

    tcaSelect(1);
    if (!lox2.begin())
    {
        Serial.println(F("Failed to boot VL53L0X #2"));
        while (1)
            ;
    }

    // Initialize the L3G4200D gyroscope
    initGyro();

    // Calibrate the gyroscope
    calibrateGyro();

    // Set initial previous time for angle calculation
    prevTime = millis();
}

void loop()
{
    VL53L0X_RangingMeasurementData_t measure;

    // Reading from Sensor 1 (Channel 0)
    tcaSelect(0);
    lox1.rangingTest(&measure, false);
    if (measure.RangeStatus != 4) // If not out of range
    {
        Serial.print("Sensor 1: ");
        Serial.print(measure.RangeMilliMeter);
        Serial.println(" mm");
    }
    else
    {
        Serial.println("Sensor 1: Out of range");
    }

    // Reading from Sensor 2 (Channel 1)
    tcaSelect(1);
    lox2.rangingTest(&measure, false);
    if (measure.RangeStatus != 4) // If not out of range
    {
        Serial.print("Sensor 2: ");
        Serial.print(measure.RangeMilliMeter);
        Serial.println(" mm");
    }
    else
    {
        Serial.println("Sensor 2: Out of range");
    }

    // Reading from Gyroscope (Channel 2)
    tcaSelect(2); // Select the gyroscope channel

    // Calculate and print the angle
    float currentTime = millis();
    currentAngle = getAngle(currentTime, &prevTime, currentAngle);

    Serial.print("Current Angle: ");
    Serial.println(currentAngle);

    delay(100); // Wait for 100ms before the next reading
}