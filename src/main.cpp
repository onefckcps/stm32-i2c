#include <Wire.h>
#include <bitset>

// 1 2 4 8 16 32 64 128 256

// Lets define bitmasks:
#define ALS_TURN_ON (0 << 0)
#define ALS_100MS_INTEGRATION_TIME (1 << 6)
#define ALS_200MS_INTEGRATION_TIME (1 << 7)
// #define ALS_400MS_INTEGRATION_TIME (ALS_100MS_INTEGRATION_TIME | ALS_200MS_INTEGRATION_TIME) // 00000011 => 11000000
#define ALS_400MS_INTEGRATION_TIME (11 << 6) // 00000011 => 11000000

// Using Bitsets:;
// std::bitset<8> ALS_TURN_ON(0 << 0);
// std::bitset<8> ALS_100MS_INTEGRATION_TIME{1 << 6};
// std::bitset<8> ALS_200MS_INTEGRATION_TIME{1 << 7};
// std::bitset<8> ALS_400MS_INTEGRATION_TIME{11 << 6};

#define VCNL4200_ADDRESS 0x51 // I2C address for VCNL4200
#define PS_SD_turniton 1 << 2

constexpr int PS_SD = {0};
// constexpr int PS_IT = {3, 2, 1};

void enablePS()
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

void enableALS()
{
  Wire.beginTransmission(VCNL4200_ADDRESS);
  Wire.write(0x00);       // Specifies which register 2 write to (in this case: ALS_CONF = 0x00; defaults to 01 => ALS disabled by defautl)
  Wire.write(0x00);       // Write 0x00 to register address 0x00 => turning ALS on
  Wire.write(0x00);       // Write 0x00 to register address 0x00 => turning ALS on
  Wire.endTransmission(); // leaving () blank or (true) will send a stop condition => allowing other masters to access to device again <=> repeated start condition: often theres the need to first write an address and after that immediately reading bytes from it. => s
}

// But with bitmanipulation / bitmasks
void enableALS2()
{
  Wire.beginTransmission(VCNL4200_ADDRESS);
  Wire.write(0x00);
  Wire.write(ALS_TURN_ON | ALS_400MS_INTEGRATION_TIME);
  Wire.write(0x00);
  Wire.endTransmission();
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

uint16_t readPSData()
{
  // Declare the function readALSData

  uint8_t psDataLow, psDataHigh;
  uint16_t psData;

  // Start I2C Transmission
  Wire.beginTransmission(VCNL4200_ADDRESS);
  // Send ALS Data Low Byte Command
  Wire.write(0x08);            // ALS Data Low Byte Register Address
  Wire.endTransmission(false); // End Transmission, but don't release the bus (Repeated Start)

  Wire.requestFrom(VCNL4200_ADDRESS, 2);

  // Read the low & high byte
  if (Wire.available())
  {
    psDataLow = Wire.read();
    psDataHigh = Wire.read();
  }

  // Combine the two bytes
  psData = (psDataHigh << 8) | psDataLow;

  return psData;
}

void setup()
{
  Wire.begin(); // Initialize I2C
  // enableALS();
  enableALS2();
  enablePS();
  Serial.begin(9600); // Start serial communication for debugging#
}

void loop()
{
  // Serial.print((ALS_TURN_ON).to_ulong());
  uint16_t alsData = readALSData();
  uint16_t psData = readPSData();
  Serial.print("ALS Data: ");
  Serial.println(alsData);
  Serial.print("PS Data: ");
  Serial.println(psData);
  delay(200); // Wait for 1 second before reading again
}
