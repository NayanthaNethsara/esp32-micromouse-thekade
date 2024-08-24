#ifndef SERIAL_MULTIPLEXER_H
#define SERIAL_MULTIPLEXER_H

class SerialMultiplexer
{
public:
    void tcaSelect(uint8_t i); // Function to select TCA9548A channel

private:
    const int TCAADDR = 0x70; // I2C address of the TCA9548A
};

#endif