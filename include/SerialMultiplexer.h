#ifndef SERIAL_MULTIPLEXER_H
#define SERIAL_MULTIPLEXER_H
#include <Arduino.h>

#include <stdint.h>

class SerialMultiplexer
{
public:
    void tcaSelect(int i);

private:
    const uint8_t TCAADDR = 0x70; // I2C address of the TCA9548A
};

#endif // SERIAL_MULTIPLEXER_H
