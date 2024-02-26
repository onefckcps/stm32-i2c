#include "pca9554.h"

uint8_t getConfig(int configAddr, int i2cAddress, int bytesToRead)
{
    uint8_t dataRegister;

    Wire.beginTransmission(i2cAddress);
    Wire.write(configAddr);
    Wire.endTransmission(false);

    Wire.requestFrom(i2cAddress, bytesToRead);

    if (Wire.available())
    {
        dataRegister = Wire.read();
    }

    return dataRegister;
}

void setConfig(int configAddr, int config) // special case
{
    uint8_t configOld = getConfig(PCA9554_CONF_REGISTER, PCA9554_ADDRESS, 1);
    configOld &= ~(config); // clear the bits (P6 = 0 => )
    Wire.beginTransmission(PCA9554_ADDRESS);
    Wire.write(configAddr);
    Wire.write(configOld);
    Wire.endTransmission();
}