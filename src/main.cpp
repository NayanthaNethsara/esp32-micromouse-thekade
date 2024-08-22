#include <Arduino.h>
#include <Wire.h>

// TCA9548A I2C address (default is 0x70)
#define TCAADDR 0x70

// L3G4200D I2C address
const int L3G4200D_ADDRESS = 0x69;

// Registers
const int L3G4200D_CTRL_REG1 = 0x20;
const int L3G4200D_OUT_Z_L = 0x2C;
const int L3G4200D_OUT_Z_H = 0x2D;
float gyroZOffset = 0.0;

// Gyroscope variables
float angle = 0;
unsigned long prevTime = 0;

// Function declarations
void initGyro();
float readGyroZ();
void calibrateGyroZ(int samples = 100);

// Function to select TCA9548A channel
void tcaSelect(uint8_t i)
{
  if (i > 7)
    return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  // Select the TCA9548A channel
  tcaSelect(1);

  // Initialize the gyroscope
  initGyro();
  calibrateGyroZ();
  // Initial time
  prevTime = millis();
}

void loop()
{
  tcaSelect(2);
  // Get current time
  unsigned long currentTime = millis();

  // Calculate delta time in seconds
  float deltaTime = (currentTime - prevTime) / 1000.0;
  prevTime = currentTime;

  // Read gyroscope data
  float zRotationRate = readGyroZ();

  // Integrate the zRotationRate to get the angle
  angle += zRotationRate * deltaTime;

  // Print the current angle for debugging
  Serial.print("Angle: ");
  Serial.println(angle);
  delay(50);
}

void initGyro()
{
  // Set CTRL_REG1 to configure the gyroscope
  Wire.beginTransmission(L3G4200D_ADDRESS);
  Wire.write(L3G4200D_CTRL_REG1);
  Wire.write(0x0F); // Normal power mode, all axes enabled, 100 Hz
  Wire.endTransmission();
}

float readGyroZ()
{
  // Read the Z-axis rotation rate
  Wire.beginTransmission(L3G4200D_ADDRESS);
  Wire.write(L3G4200D_OUT_Z_L | 0x80); // Set auto-increment bit
  Wire.endTransmission();

  Wire.requestFrom(L3G4200D_ADDRESS, 2);
  while (Wire.available() < 2)
    ;

  uint8_t zL = Wire.read();
  uint8_t zH = Wire.read();

  int16_t z = (zH << 8) | zL;

  // Subtract the calibrated offset
  z -= gyroZOffset;

  // Convert to degrees per second
  float zRotationRate = z * 0.00875; // 0.00875 dps/LSB for 250 dps full scale

  return zRotationRate;
}

void calibrateGyroZ(int samples = 100)
{
  long sum = 0;
  for (int i = 0; i < samples; i++)
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
    sum += z;

    delay(10); // Small delay to allow for stable readings
  }
  gyroZOffset = sum / (float)samples;
}