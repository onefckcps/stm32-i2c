#include "VCNL4200.h"

// TODO: fix this
void VCNL4200::enablePS()
{
    // Write to PS_CONF 1 & PS_CONF 2 Registers
    Wire.beginTransmission(VCNL4200_ADDRESS);
    Wire.write(0x03);
    Wire.write(0x00);   // enables PS functionality
    Wire.write(0b1000); // enables 16bit for PS_OUTPUT
    Wire.endTransmission(false);

    // Write to PS_MS Register
    Wire.beginTransmission(VCNL4200_ADDRESS);
    Wire.write(0x04);
    Wire.write(0x00);
    Wire.write(0b10000); // in theory: should enable proximity normal operation mode
    Wire.endTransmission();
}

// Enable VCNL4200 using bitmanipulation / bitmasks; 400ms IT
void VCNL4200::enableALS()
{
    Wire.beginTransmission(VCNL4200_ADDRESS);
    Wire.write(0x00);
    Wire.write(ALS_TURN_ON | ALS_400MS_INTEGRATION_TIME);
    Wire.write(0x00);
    Wire.endTransmission();
}

uint16_t VCNL4200::readData(uint8_t registerAddress)
{
    uint8_t lowData, highData;
    uint16_t data;

    // First send register address you want to read from
    Wire.beginTransmission(VCNL4200_ADDRESS);
    Wire.write(registerAddress);
    Wire.endTransmission(false); // repeated start condition

    // Now we can request data from the sensor
    Wire.requestFrom(VCNL4200_ADDRESS, 2);

    if (Wire.available())
    {
        lowData = Wire.read();
        highData = Wire.read();
    }

    data = (highData << 8) | lowData;

    return data;
}

uint16_t VCNL4200::readALSData()
{
    return readData(0x09);
}

uint16_t VCNL4200::readPSData()
{
    return readData(0x08);
}
