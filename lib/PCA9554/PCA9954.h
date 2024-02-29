#ifndef PCA9554_H
#define PCA9554_H
#include <cstdint>
#include <Wire.h>
#include <bitset>

#define PCA9554_ADDRESS 0x27       // Port expander address => A0 A1 A2 all connected to 3V3 => all in 'High' state (datasheet page 18)
#define PCA9554_CONF_REGISTER 0x03 // Configuration Register => datasheet p.19
#define P6_OUTPUT (1 << 6)         // control WHITE LED

// // functions declarations
// uint8_t getConfig(int configAddr, int i2cAddress, int bytesToRead);

// void setConfig(int addr, int config);

#endif

class PCA9554
{

public:
    // Default Constructor
    // PCA9554(); // dont need this constructor
    uint8_t getConfig(int configAddr, int i2cAddress, int bytesToRead);
    void setConfig(int addr, int config, int zeroOrOne);
    void led_white_on(PCA9554 &obj);
    void led_white_off(PCA9554 &obj);
    void resetCommandConfig(PCA9554 &obj);
};
