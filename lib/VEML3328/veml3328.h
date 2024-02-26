// Header guards:
#ifndef VEML3328
#define VEML3328

#include <bitset>
#include <Wire.h>

// Lets define functions
void veml3328_enableSensor(); // sets register SD0 & SD1 to 0 => enabled
uint16_t readBlue();
uint16_t readGreen();
uint16_t readRed();

// Slave address
#define VEML3328_ADDRESS 0x10

// Bitmasks
#define SD0_POWER_ON (0 << 0)
#define SD1_POWER_ON (0 << 15)

#endif