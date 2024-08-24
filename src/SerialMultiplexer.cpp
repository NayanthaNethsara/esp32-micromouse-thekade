#include "SerialMultiplexer.h"
#include <Arduino.h>
#include <Wire.h>

// Function to select TCA9548A channel
void SerialMultiplexer::tcaSelect(int i)
{
    if (i > 7)
        return;
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}