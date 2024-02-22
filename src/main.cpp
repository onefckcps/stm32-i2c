#include <Wire.h>

#define VCNL4200_ADDRESS 0x51 // I2C address for VCNL4200

uint16_t readALSData();

uint16_t readALSData()
{
  // Declare the function readALSData

  uint8_t alsDataLow, alsDataHigh;
  uint16_t alsData;

  // Start I2C Transmission
  Wire.beginTransmission(VCNL4200_ADDRESS);
  // Send ALS Data Low Byte Command
  Wire.write(0x09);            // ALS Data Low Byte Register Address
  Wire.endTransmission(false); // End Transmission, but don't release the bus (Repeated Start)

  // Request 1 byte of data
  Wire.requestFrom(VCNL4200_ADDRESS, 1);
  // Read the low byte
  if (Wire.available() == 1)
  {
    alsDataLow = Wire.read();
  }

  // Start I2C Transmission
  Wire.beginTransmission(VCNL4200_ADDRESS);
  // Send ALS Data High Byte Command
  Wire.write(0x0A);            // ALS Data High Byte Register Address
  Wire.endTransmission(false); // End Transmission, but don't release the bus (Repeated Start)

  // Request 1 byte of data
  Wire.requestFrom(VCNL4200_ADDRESS, 1);
  // Read the high byte
  if (Wire.available() == 1)
  {
    alsDataHigh = Wire.read();
  }

  // Combine the two bytes
  alsData = (alsDataHigh << 8) | alsDataLow;

  return alsData;
}

void setup()
{
  Wire.begin();       // Initialize I2C
  Serial.begin(9600); // Start serial communication for debugging#
}

void loop()
{
  uint16_t alsData = readALSData();
  Serial.print("ALS Data: ");
  Serial.println(alsData);
  delay(1000); // Wait for 1 second before reading again
}
