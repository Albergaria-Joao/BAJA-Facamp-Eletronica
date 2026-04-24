#include "max6675.h"

int CLK = 13;
int CS = 10;
int SO = 12;

MAX6675 temp_sensor(CLK, CS, SO);

void setup() 
{
 Serial.begin(9600);
 delay(500);
}

void loop() 
{
  float temp_C = temp_sensor.readCelsius();
  float temp_F = temp_sensor.readFahrenheit();

  Serial.print("temp_C = ");
  Serial.print(temp_C);

  Serial.print("   temp_F = ");
  Serial.println(temp_F);

  delay(500);
}
