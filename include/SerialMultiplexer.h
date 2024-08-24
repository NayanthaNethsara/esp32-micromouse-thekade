#ifndef SERIAL_MULTIPLEXER_H
#define SERIAL_MULTIPLEXER_H
#include <Arduino.h>

class SerialMultiplexer
{
public:
    void tcaSelect(int i);

private:
    const int TCAADDR = 0x70; // I2C address of the TCA9548A
};

#endif