// Header guards:
#ifndef VCNL4200
#define VCNL4200

#include <bitset>
#include <Wire.h>

// Lets define bitmasks:
#define ALS_TURN_ON (0 << 0)
#define ALS_100MS_INTEGRATION_TIME (1 << 6)
#define ALS_200MS_INTEGRATION_TIME (1 << 7)
// #define ALS_400MS_INTEGRATION_TIME (ALS_100MS_INTEGRATION_TIME | ALS_200MS_INTEGRATION_TIME) // 00000011 => 11000000
#define ALS_400MS_INTEGRATION_TIME (11 << 6) // 00000011 => 11000000

#define VCNL4200_ADDRESS 0x51 // I2C address for VCNL4200

// Lets define our functions
void enablePS();
void enableALS();
void enableALS2();
uint16_t readALSData();
uint16_t readPSData();

#endif