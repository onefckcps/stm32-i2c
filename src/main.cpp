// #include "vcnl4200.h"
#include "veml3328.h"
#include "pca9554.h"

void setup()
{
  Wire.begin(); // Initialize I2C
  // enableALS();
  // enableALS2();
  // enablePS();
  // enableSensor();
  setConfig(PCA9554_CONF_REGISTER, P6_OUTPUT);
  Serial.begin(9600); // Start serial communication for debugging#
  // Serial.println("Setup beendet");
}

void loop()
{
  // Serial.print((ALS_TURN_ON).to_ulong());
  // uint16_t alsData = readALSData();
  // uint16_t psData = readPSData();
  // // Serial.print("ALS Data: ");
  // Serial.println(alsData);
  // Serial.print("PS Data: ");
  // Serial.println(psData);
  // uint16_t blueData = readBlue();
  // Serial.println(blueData);
  uint16_t greenData = readGreen();
  uint16_t blueData = readBlue();
  uint16_t redData = readRed();
  // Serial.print(redData);
  // Serial.print(",");
  // Serial.print(greenData);
  // Serial.print(",");
  // Serial.println(blueData);

  // trying to read command Register
  uint8_t data = getConfig(PCA9554_CONF_REGISTER, PCA9554_ADDRESS, 1);
  Serial.println(std::bitset<8>(data).to_string().c_str()); // to_string() returns a string representation of the bitset c_str() returns a pointer to an array that contains a null-terminated sequence of characters (i.e., a C-string) representing the current value of the string object.

  delay(200); // Wait for 1 second before reading again
}
