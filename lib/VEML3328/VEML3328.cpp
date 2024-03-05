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

uint16_t VEML3328::readIr()
{
    return readRGB(0x08);
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

// first tests
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

VEML3328::RGB VEML3328::calculateAverageRGB(const std::vector<VEML3328::RGB> &rgbValues)
{
    RGB averageRGB = {0, 0, 0};

    if (!rgbValues.empty())
    {
        unsigned long sumRed = 0;
        unsigned long sumGreen = 0;
        unsigned long sumIr = 0;

        for (const auto &rgb : rgbValues)
        {
            sumRed += rgb.red;
            sumGreen += rgb.green;
            sumIr += rgb.ir;
        }

        averageRGB.red = sumRed / rgbValues.size();
        averageRGB.green = sumGreen / rgbValues.size();
        averageRGB.ir = sumIr / rgbValues.size();
    }

    return averageRGB;
}

std::vector<VEML3328::RGB> VEML3328::readRGBFor3Sec()
{
    std::vector<RGB> rgbValues;

    unsigned long startTime = millis();
    while (millis() - startTime < 3000)
    { // 3000 milliseconds = 3 seconds
        RGB rgb;
        rgb.red = readRed();
        rgb.green = readGreen();
        rgb.ir = readIr();
        rgbValues.push_back(rgb);

        delay(100); // Wait for 100 milliseconds before the next reading
    }

    return rgbValues;
}

std::string VEML3328::calculateGreenToIR(const RGB &averageRGB)
{
    if (averageRGB.green == 0 || averageRGB.ir == 0)
    {
        return "Division by zero error";
    }

    float ratio = static_cast<float>(averageRGB.green) / averageRGB.ir;

    if (ratio < 0.045)
    {
        return "grün " + std::to_string(ratio);
    }
    else if (0.045 < ratio && ratio < 0.07)
    {
        return "braun " + std::to_string(ratio);
    }
    else if (ratio > 0.07)
    {
        return "gelb " + std::to_string(ratio);
    }
    else if (ratio < 0.045 || ratio > 0.07)
    {
        return "out of range :/ + std::to_string(ratio)";
    }
}

std::string VEML3328::performMeasurement()
{
    std::vector<VEML3328::RGB> rgbValues = readRGBFor3Sec();
    RGB averageRGB = calculateAverageRGB(rgbValues);
    std::string result = calculateGreenToIR(averageRGB); //+ "," + std::to_string(averageRGB.green) + "," + std::to_string(averageRGB.ir);

    return result;
}