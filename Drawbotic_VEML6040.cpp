#include "Drawbotic_VEML6040.h"

#include <math.h>

uint16_t Drawbotic_VEML6040::IntegrationTimeToMSec(VEML6040_IntegrationTime intTime)
{
  switch(intTime)
  {
  case VEML6040_IT_40MS:
    return 40;
  case VEML6040_IT_80MS:
    return 80;
  case VEML6040_IT_160MS:
    return 160;
  case VEML6040_IT_320MS:
    return 320;
  case VEML6040_IT_640MS:
    return 640;
  case VEML6040_IT_1280MS:
    return 1280;
  default:
    return 1;
  }
}

void Drawbotic_VEML6040::i2cRead(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
  Wire.beginTransmission(dev_addr);
  Wire.write(reg_addr);
  Wire.endTransmission();

  Wire.requestFrom(dev_addr, len);
  for(int i = 0; i < len; i++)
  {
    data[i] = Wire.read();
  }
}

void Drawbotic_VEML6040::i2cWrite(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
  Wire.beginTransmission(dev_addr);

  Wire.write(reg_addr);
  for(int i = 0; i < len; i++)
  {
    Wire.write(data[i]);
  }
  Wire.endTransmission();
}

Drawbotic_VEML6040::Drawbotic_VEML6040()
{

}

void Drawbotic_VEML6040::initialise(VEML6040_IntegrationTime intTime)
{
  Wire.begin();
  
  uint8_t data[2] = { (uint8_t)intTime, 0x00 };
  i2cWrite(VEML6040_ADDR, VEML6040_REG_CONF, data, 2);
  m_intTime = intTime;
}

uint16_t Drawbotic_VEML6040::getCurrentIntegrationTime()
{
  return IntegrationTimeToMSec(m_intTime);
}

VEML6040_Colour Drawbotic_VEML6040::getColour()
{
  uint8_t data[2];
  VEML6040_Colour result;
  //read red
  i2cRead(VEML6040_ADDR, VEML6040_REG_R_DATA, data, 2);
  result.red = ((uint16_t)data[1]) << 8 | data[0];

  i2cRead(VEML6040_ADDR, VEML6040_REG_G_DATA, data, 2);
  result.green = ((uint16_t)data[1]) << 8 | data[0];

  i2cRead(VEML6040_ADDR, VEML6040_REG_B_DATA, data, 2);
  result.blue = ((uint16_t)data[1]) << 8 | data[0];

  i2cRead(VEML6040_ADDR, VEML6040_REG_W_DATA, data, 2);
  result.white = ((uint16_t)data[1]) << 8 | data[0];

  return result;
}

float Drawbotic_VEML6040::getCCT(float offset)
{
  VEML6040_Colour reading = getColour();

  float ccti;

  ccti = (float)reading.red - (float)reading.blue / (float)reading.green;
  ccti += offset;

  return 4278.6 * pow(ccti, -1.2455);
}

float Drawbotic_VEML6040::getAmbientLux()
{
  uint8_t data[2];
  uint16_t green;
  float lux;

  i2cRead(VEML6040_ADDR, VEML6040_REG_G_DATA, data, 2);
  green = ((uint16_t)data[1]) << 8 | data[0];

  switch(m_intTime)
  {
  case VEML6040_IT_40MS:
    lux = green * VEML6040_GSENS_40MS;
    break;
  case VEML6040_IT_80MS:
    lux = green * VEML6040_GSENS_80MS;
    break;
  case VEML6040_IT_160MS:
    lux = green * VEML6040_GSENS_160MS;
    break;
  case VEML6040_IT_320MS:
    lux = green * VEML6040_GSENS_320MS;
    break;
  case VEML6040_IT_640MS:
    lux = green * VEML6040_GSENS_640MS;
    break; 
  case VEML6040_IT_1280MS:
    lux = green * VEML6040_GSENS_1280MS; 
    break;
  default:
    lux = -1;
    break;
  }

  return lux;
}