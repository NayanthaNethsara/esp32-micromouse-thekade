#include "SerialMultiplexer.h"
#include <Wire.h>

// Function to select TCA9548A channel
void SerialMultiplexer::tcaSelect(uint8_t i)
{
    if (i > 7)
        return;
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}