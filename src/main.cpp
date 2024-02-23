#include <Wire.h>

#define VCNL4200_ADDRESS 0x51 // I2C address for VCNL4200

void enableALS()
{
  Wire.beginTransmission(VCNL4200_ADDRESS);
  Wire.write(0x00);       // Specifies which register 2 write to (in this case: ALS_CONF = 0x00; defaults to 01 => ALS disabled by defautl)
  Wire.write(0x00);       // Write 0x00 to register address 0x00 => turning ALS on
  Wire.write(0x00);       // Write 0x00 to register address 0x00 => turning ALS on
  Wire.endTransmission(); // leaving () blank or (true) will send a stop condition => allowing other masters to access to device again <=> repeated start condition: often theres the need to first write an address and after that immediately reading bytes from it. => s
}

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

  Wire.requestFrom(VCNL4200_ADDRESS, 2);

  // Read the low & high byte
  if (Wire.available())
  {
    alsDataLow = Wire.read();
    alsDataHigh = Wire.read();
  }

  // Combine the two bytes
  alsData = (alsDataHigh << 8) | alsDataLow;

  return alsData;
}

void setup()
{
  Wire.begin(); // Initialize I2C
  enableALS();
  Serial.begin(9600); // Start serial communication for debugging#
}

void loop()
{
  uint16_t alsData = readALSData();
  Serial.print("ALS Data: ");
  Serial.println(alsData);
  delay(1000); // Wait for 1 second before reading again
}
