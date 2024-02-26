// Header guards:
#ifndef VEML3328_H
#define VEML3328_H

#include <bitset>
#include <Wire.h>

// Slave address
#define VEML3328_ADDRESS 0x10

// Bitmasks
#define SD0_POWER_ON (0 << 0)
#define SD1_POWER_ON (0 << 15)

class VEML3328 final
{

public:
    VEML3328();
    // Lets define functions
    void enableSensor();  // sets register SD0 & SD1 to 0 => enabled
    uint16_t readBlue();  // raw count number of blue light
    uint16_t readGreen(); // raw count number of green light
    uint16_t readRed();   // raw count number of red light => convert to lux
};

#endif