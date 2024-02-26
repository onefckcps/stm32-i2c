#include "pca9554.h"

uint8_t getConfig(int addr)
{
    uint8_t dataRegister;

    Wire.beginTransmission(PCA9554_ADDRESS);
    Wire.write(addr);
    Wire.endTransmission(false);

    Wire.requestFrom(PCA9554_ADDRESS, 1);

    if (Wire.available())
    {
        dataRegister = Wire.read();
    }

    return dataRegister;
}

void setConfig(int configAddr, int config) // special case
{
    uint8_t configOld = getConfig(configAddr);
    configOld &= ~(config); // clear the bits (P6 = 0 => )
    Wire.beginTransmission(PCA9554_ADDRESS);
    Wire.write(configAddr);
    Wire.write(configOld);
    Wire.endTransmission();
}