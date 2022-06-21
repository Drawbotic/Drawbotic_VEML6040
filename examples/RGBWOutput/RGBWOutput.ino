#include <Drawbotic_VEML6040.h>

Drawbotic_VEML6040 sensor;

void setup()
{
  Serial.begin(9600);
  sensor.initialise(VEML6040_IT_40MS);
}

void loop()
{
  VEML6040_Colour reading = sensor.getColour();

  Serial.print("R: "); Serial.print(reading.red);
  Serial.print("\tG: "); Serial.print(reading.green);
  Serial.print("\tB: "); Serial.print(reading.blue);
  Serial.print("\tW: "); Serial.println(reading.white);

  delay(sensor.getCurrentIntegrationTime());
}