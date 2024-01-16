#include "Drawbotic_VEML6040.h"

#include <math.h>
#include <Arduino.h>

/*!
 * \brief Converts a VEML6040_IntegrationTime enum value to milliseconds
 * 
 * \param intTime The VEML6040_IntegrationTime to convert
 * \return The value of intTime in milliseconds
 */
uint16_t Drawbotic_VEML6040::IntegrationTimeToMSec(VEML6040_IntegrationTime intTime) {
  switch(intTime) {
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

uint16_t Drawbotic_VEML6040::read(uint8_t reg) {
  uint16_t data = 0;

  Wire.beginTransmission(VEML6040_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(VEML6040_ADDR, 2);
  while(Wire.available()) {
    data = Wire.read();
    data |= Wire.read() << 8;
  }

  return data;
}

/*!
 * \brief Construct a new Drawbotic_VEML6040 object
 * 
 */
Drawbotic_VEML6040::Drawbotic_VEML6040()
{}

/*!
 * \brief Starts the VEML6040 sensor
 * 
 * \return true - Sensor started successfully
 * \return false - Sensor start up failed
 */
bool Drawbotic_VEML6040::begin(void) {
  bool sensorExists;
  Wire.begin();
  Wire.beginTransmission(VEML6040_ADDR);
  if (Wire.endTransmission() == 0) {
    sensorExists = true;
  }
  return sensorExists;
}

/*!
 * \brief Sets the configuration for the VEML6040 sensor
 * 
 * \param intTime The desired Integration Time, see VEML6040_IntegrationTime enum
 * \param force (Default: false) Enables force mode (readings only occur when trigger is set true), otherwise auto mode is used.
 * \param trig (Default: false) When true triggers a reading when force is also true
 * \param disabled (Default: false) when true disables all reading
 */
void Drawbotic_VEML6040::setConfig(VEML6040_IntegrationTime intTime, bool force, bool trig, bool disabled) { 
  uint8_t sensorEnabled = disabled ? VEML6040_SD_DISABLE : VEML6040_SD_ENABLE;
  uint8_t mode = force ? VEML6040_AF_FORCE : VEML6040_AF_AUTO;
  uint8_t trigger = trig ? VEML6040_TRIG_ENABLE : VEML6040_TRIG_DISABLE;

  uint8_t conf = (uint8_t)intTime + sensorEnabled + mode + trigger;

  Wire.beginTransmission(VEML6040_ADDR);
  Wire.write(VEML6040_REG_CONF);
  Wire.write(conf);
  Wire.write(0);
  Wire.endTransmission();

  m_intTime = intTime;
}

/*!
 * \brief Returns the current Integration Time in milliseconds
 * 
 * \return Current Integration time in milliseconds 
 */
uint16_t Drawbotic_VEML6040::getCurrentIntegrationTime() {
  return IntegrationTimeToMSec(m_intTime);
}

/*!
 * \brief Reads the current RGBW value from the VEML6040 sensor
 * 
 * \return A VEML6040 struct containing the RGBW data
 */
VEML6040_Colour Drawbotic_VEML6040::getColour() {
  VEML6040_Colour result;
  
  result.red = read(VEML6040_REG_R_DATA);
  result.green = read(VEML6040_REG_G_DATA);
  result.blue = read(VEML6040_REG_B_DATA);
  result.white = read(VEML6040_REG_W_DATA);

  return result;
}

/*!
 * \brief Reads the VEML6040 sensor and calculates the Correlated Colour Temperature (CCT) based on that reading
 * 
 * \param offset (Default: 0.5) An offset used in the CCT calculation, open air is calibrated to 0.5, different coverings might change this
 * \return The calculated CCT
 */
float Drawbotic_VEML6040::getCCT(float offset) {
  VEML6040_Colour reading = getColour();

  float ccti;

  ccti = (float)reading.red - (float)reading.blue / (float)reading.green;
  ccti += offset;

  return 4278.6 * pow(ccti, -1.2455);
}

/*!
 * \brief Reads the VEML6040 sensor and calculates the Ambient Lux level from that reading
 * 
 * \return The calculated Lux value
 */
float Drawbotic_VEML6040::getAmbientLux() {
  uint16_t green;
  float lux;

  green = read(VEML6040_REG_G_DATA);

  switch(m_intTime) {
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