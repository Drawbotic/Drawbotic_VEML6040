#ifndef DRAWBOTIC_VEML6040_H
#define DRAWBOTIC_VEML6040_H

#include <Wire.h>

#define VEML6040_ADDR         0x10
#define VEML6040_REG_CONF     0x00
#define VEML6040_REG_R_DATA   0x08
#define VEML6040_REG_G_DATA   0x09
#define VEML6040_REG_B_DATA   0x0A
#define VEML6040_REG_W_DATA   0x0B

#define VEML6040_GSENS_40MS   0.25168
#define VEML6040_GSENS_80MS   0.12584
#define VEML6040_GSENS_160MS  0.06292
#define VEML6040_GSENS_320MS  0.03146
#define VEML6040_GSENS_640MS  0.01573
#define VEML6040_GSENS_1280MS 0.007865

typedef enum 
{
  VEML6040_IT_40MS   = 0x00,
  VEML6040_IT_80MS   = 0x10,
  VEML6040_IT_160MS  = 0x20,
  VEML6040_IT_320MS  = 0x30,
  VEML6040_IT_640MS  = 0x40,
  VEML6040_IT_1280MS = 0x50,
}
VEML6040_IntegrationTime;

struct VEML6040_Colour
{
  uint16_t red;
  uint16_t green;
  uint16_t blue;
  uint16_t white;
};
typedef VEML6040_Colour VEML6040_Color; //for the yanks

class Drawbotic_VEML6040 
{
public:
  Drawbotic_VEML6040();
  void initialise(VEML6040_IntegrationTime intTime);
  uint16_t getCurrentIntegrationTime();
  VEML6040_Colour getColour();
  VEML6040_Colour getColor() { return getColour(); }  //...again for the yanks
  float getCCT(float offset = 0.5f);
  float getAmbientLux();

  static uint16_t IntegrationTimeToMSec(VEML6040_IntegrationTime intTime);
private:
  VEML6040_IntegrationTime m_intTime;
  void i2cRead(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
  void i2cWrite(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
};

#endif