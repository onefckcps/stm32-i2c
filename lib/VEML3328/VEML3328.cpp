#include "VEML3328.h"
#include <Arduino.h> // Include the necessary header file

VEML3328::VEML3328()
{
    enableSensor();
    Serial.println("VEML3328 object created");
}

void VEML3328::enableSensor() // sets register SD0 & SD1 to 0 => enabled
{
    Wire.beginTransmission(VEML3328_ADDRESS);
    Wire.write(0x00); // address for command code register
    Wire.write(SD0_POWER_ON);
    Wire.write(SD1_POWER_ON);
    Wire.endTransmission();
}

uint16_t VEML3328::readRGB(uint8_t registerAddress)
{
    uint8_t lowData, highData;
    uint16_t data;

    // First send register address you want to read from
    Wire.beginTransmission(VEML3328_ADDRESS);
    Wire.write(registerAddress);
    Wire.endTransmission(false); // repeated start condition

    // Now we can request data from the sensor
    Wire.requestFrom(VEML3328_ADDRESS, 2);

    if (Wire.available())
    {
        lowData = Wire.read();
        highData = Wire.read();
    }

    data = (highData << 8) | lowData;

    return data;
}

uint16_t VEML3328::readBlue()
{
    return readRGB(0x07);
}

uint16_t VEML3328::readGreen()
{
    return readRGB(0x06);
}

uint16_t VEML3328::readRed()
{
    return readRGB(0x05);
}

uint8_t VEML3328::normalize(uint16_t data)
{
    // uint8_t factor = (2 ^ (16)); //* 0.003;

    return (uint8_t)(((float)data / 30000.0) * 255);
}

// readBlue, readGreen, readRed normalized to 8 bit
uint8_t VEML3328::readBlueNormalized()
{
    return normalize(readBlue());
}

uint8_t VEML3328::readGreenNormalized()
{
    return normalize(readGreen());
}

uint8_t VEML3328::readRedNormalized()
{
    return normalize(readRed());
}

bool VEML3328::isBananaYellow()
{
    uint8_t red = readRed();
    uint8_t green = readGreen();
    uint8_t blue = readBlue();
    ;

    if (red && green != 0)
    {
        uint8_t redBlue = blue / red;
        uint8_t greenBlue = blue / green;
        if (redBlue > greenBlue * 0.3)
        {
            bananaIsYellow = true;
        }
        else
        {
            bananaIsYellow = false;
        }
        return bananaIsYellow;
    }
    else
    {
        return false;
    }
}

bool VEML3328::isBananaYellowTest()
{
    uint8_t red = readRed();
    uint8_t green = readGreen();
    uint8_t blue = readBlue();

    uint8_t redBlue = blue / red;
    uint8_t greenBlue = blue / green;

    return bananaIsYellow;
}