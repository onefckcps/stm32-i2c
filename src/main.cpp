#include <Arduino.h>
#include <Wire.h>

// put function declarations here:
int myFunction(int, int);

int slaveAddress;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(); // Initiate I2C communication
  slaveAddress = 0x51;

  Wire.beginTransmission(slaveAddress);
  Wire.requestFrom(address, quantity);

  //   pinMode(D15, OUTPUT);
  //   digitalWrite(D15, HIGH);

  //   pinMode(D14, OUTPUT);
  //   digitalWrite(D14, HIGH);

  Wire.endTransmission();
}

// Serial.print("Result: ");

void loop()
{
  // put your main code here, to run repeatedly:
  // for (int i = 0; i < 1000; i += 10)
  // {
  //   Serial.print(i);
  // }
  // for (int i = 1000; i > 0; i -= 10)
  // {
  //   Serial.print(i);
  // }
}

// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}