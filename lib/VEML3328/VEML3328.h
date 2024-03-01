// Header guards:
#ifndef VEML3328_H
#define VEML3328_H

#include <bitset>
#include <Wire.h>
#include <vector>
#include <Arduino.h> // Include the necessary header file

// Slave address
#define VEML3328_ADDRESS 0x10

// Bitmasks
#define SD0_POWER_ON (0 << 0)
#define SD1_POWER_ON (0 << 15)

class VEML3328 final
{
private:
    uint16_t readRGB(uint8_t registerAddress);

    // Lets normalize the data
    uint8_t normalize(uint16_t data);
    bool bananaIsYellow = false;

    struct RGB
    {
        uint16_t red;
        uint16_t green;
        uint16_t ir;
    };

public:
    VEML3328();
    // Lets define functions
    void enableSensor();  // sets register SD0 & SD1 to 0 => enabled
    uint16_t readBlue();  // raw count number of blue light
    uint16_t readGreen(); // raw count number of green light
    uint16_t readRed();   // raw count number of red light => convert to lux
    uint16_t readIr();

    // Lets normalize the data
    uint8_t readBlueNormalized();
    uint8_t readGreenNormalized();
    uint8_t readRedNormalized();

    // Lets determine the color of the bannana
    bool isBananaYellow();
    bool isBananaYellowTest();

    std::vector<RGB> readRGBFor3Sec();
    RGB calculateAverageRGB(const std::vector<RGB> &rgbValues);
    std::string calculateGreenToIR(const RGB &averageRGB);
    std::string performMeasurement();
};

#endif