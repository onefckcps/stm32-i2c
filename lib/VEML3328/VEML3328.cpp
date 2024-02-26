#include "VEML3328.h"

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

uint16_t VEML3328::readBlue()
{
    uint16_t data = readRGB(0x07);
    return data;
}

uint16_t VEML3328::readGreen()
{
    uint16_t data = readRGB(0x06);
    return data;
}

uint16_t VEML3328::readRed()
{
    uint16_t data = readRGB(0x05);
    return data;
}

// Function to read RGB data from the sensor using the command code
uint8_t VEML3328::readRGB(int commandCode)
{
    uint8_t lowData, highData = 0;
    uint16_t data = 0;

    // First send register address you want to read from
    Wire.beginTransmission(VEML3328_ADDRESS);
    Wire.write(commandCode);
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
