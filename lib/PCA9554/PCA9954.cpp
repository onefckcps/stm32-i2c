#include "PCA9954.h"
uint8_t PCA9554::getConfig(int configAddr, int i2cAddress, int bytesToRead)
{
    uint8_t dataRegister = 0;

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

void PCA9554::setConfig(int configAddr, int config, int zeroOrOne) // special case
{
    if (zeroOrOne == 0)
    {
        uint8_t configOld = getConfig(PCA9554_CONF_REGISTER, PCA9554_ADDRESS, 1);
        configOld &= ~(config); // clear the bits (P6 = 0 => )
        Wire.beginTransmission(PCA9554_ADDRESS);
        Wire.write(configAddr);
        Wire.write(configOld);
        Wire.endTransmission();
    }
    else if (zeroOrOne == 1)
    {
        uint8_t configOld = getConfig(PCA9554_CONF_REGISTER, PCA9554_ADDRESS, 1);
        configOld |= config;
        Wire.beginTransmission(PCA9554_ADDRESS);
        Wire.write(configAddr);
        Wire.write(configOld);
        Wire.endTransmission();
    }
}

void PCA9554::led_white_on(PCA9554 &obj)
{
    obj.setConfig(PCA9554_CONF_REGISTER, P6_OUTPUT, 0);
}

void PCA9554::led_white_off(PCA9554 &obj)
{
    obj.setConfig(PCA9554_CONF_REGISTER, P6_OUTPUT, 1);
}

void PCA9554::led_ir_on(PCA9554 &obj)
{
    obj.setConfig(PCA9554_CONF_REGISTER, P2_OUTPUT, 0);
}

void PCA9554::led_red_on(PCA9554 &obj)
{
    obj.setConfig(PCA9554_CONF_REGISTER, P3_OUTPUT, 0);
}

void PCA9554::led_green_on(PCA9554 &obj)
{
    obj.setConfig(PCA9554_CONF_REGISTER, P4_OUTPUT, 0);
}

void PCA9554::resetCommandConfig(PCA9554 &obj)
{
    obj.setConfig(PCA9554_CONF_REGISTER, 255, 1);
}