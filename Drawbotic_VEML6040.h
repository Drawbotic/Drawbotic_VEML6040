#ifndef DRAWBOTIC_VEML6040_H
#define DRAWBOTIC_VEML6040_H

#include <Wire.h>

#define VEML6040_ADDR         0x10
#define VEML6040_REG_CONF     0x00
#define VEML6040_REG_R_DATA   0x08
#define VEML6040_REG_G_DATA   0x09
#define VEML6040_REG_B_DATA   0x0A
#define VEML6040_REG_W_DATA   0x0B

#define VEML6040_TRIG_DISABLE 0x00
#define VEML6040_TRIG_ENABLE  0x04

#define VEML6040_AF_AUTO      0x00
#define VEML6040_AF_FORCE     0x02

#define VEML6040_SD_ENABLE    0x00
#define VEML6040_SD_DISABLE   0x01 

// G SENSITIVITY

#define VEML6040_GSENS_40MS   0.25168
#define VEML6040_GSENS_80MS   0.12584
#define VEML6040_GSENS_160MS  0.06292
#define VEML6040_GSENS_320MS  0.03146
#define VEML6040_GSENS_640MS  0.01573
#define VEML6040_GSENS_1280MS 0.007865

enum VEML6040_IntegrationTime {
  VEML6040_IT_40MS   = 0x00,
  VEML6040_IT_80MS   = 0x10,
  VEML6040_IT_160MS  = 0x20,
  VEML6040_IT_320MS  = 0x30,
  VEML6040_IT_640MS  = 0x40,
  VEML6040_IT_1280MS = 0x50,
};

struct VEML6040_Colour {
  float red;
  float green;
  float blue;
  float white;
};
typedef VEML6040_Colour VEML6040_Color; //for the yanks

class Drawbotic_VEML6040 {
public:
  Drawbotic_VEML6040();
  bool begin();
  void setConfig(VEML6040_IntegrationTime intTime, bool force = false, bool trig = false, bool disabled = false);
  uint16_t getCurrentIntegrationTime();
  VEML6040_Colour getColour();
  VEML6040_Colour getColor() { return getColour(); }  //...again for the yanks
  float getCCT(float offset = 0.5f);
  float getAmbientLux();
  
  static uint16_t IntegrationTimeToMSec(VEML6040_IntegrationTime intTime);

private:
  VEML6040_IntegrationTime m_intTime;
  uint16_t read(uint8_t reg);
};

#endif