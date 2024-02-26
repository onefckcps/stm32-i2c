#include "veml3328.h"

void veml3328_enableSensor() // sets register SD0 & SD1 to 0 => enabled
{
    Wire.beginTransmission(VEML3328_ADDRESS);
    Wire.write(0x00); // address for command code register
    Wire.write(SD0_POWER_ON);
    Wire.write(SD1_POWER_ON);
    Wire.endTransmission();
}

uint16_t readBlue()
{
    uint8_t lowData, highData;
    uint16_t data;

    // First send register address you want to read from
    Wire.beginTransmission(VEML3328_ADDRESS);
    Wire.write(0x07);
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

uint16_t readGreen()
{
    uint8_t lowData, highData;
    uint16_t data;

    // First send register address you want to read from
    Wire.beginTransmission(VEML3328_ADDRESS);
    Wire.write(0x06);
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

uint16_t readRed()
{
    uint8_t lowData, highData;
    uint16_t data;

    // First send register address you want to read from
    Wire.beginTransmission(VEML3328_ADDRESS);
    Wire.write(0x05);
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
