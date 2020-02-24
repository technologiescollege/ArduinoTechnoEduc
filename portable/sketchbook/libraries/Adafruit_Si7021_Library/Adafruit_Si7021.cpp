/*!
 * @file Adafruit_Si7021.cpp
 *
 *  @mainpage Adafruit Si7021 breakout board
 *
 *  @section intro_sec Introduction
 *
 *  This is a library for the Si7021 Temperature & Humidity Sensor.
 *
 *  Designed specifically to work with the Adafruit Si7021 Breakout Board.
 *
 *  Pick one up today in the adafruit shop!
 *  ------> https://www.adafruit.com/product/3251
 *
 *  These sensors use I2C to communicate, 2 pins are required to interface.
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit andopen-source hardware by purchasing products
 *  from Adafruit!
 *
 *  @section author Author
 *
 *  Limor Fried (Adafruit Industries)
 *
 *  @section license License
 *
 *  BSD license, all text above must be included in any redistribution
 */

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Si7021.h>

/*!
 *  @brief  Instantiates a new Adafruit_Si7021 class
 *  @param  *theWire
 *          optional wire object
 */
Adafruit_Si7021::Adafruit_Si7021(TwoWire *theWire) {
  _i2caddr = SI7021_DEFAULT_ADDRESS;
  _wire = theWire;
  sernum_a = sernum_b = 0;
  _model = SI_7021;
  _revision = 0;
}


/*!
 *  @brief  Sets up the HW by reseting It, reading serial number and reading revision.
 *  @return Returns true if set up is successful.
 */
bool Adafruit_Si7021::begin() {
  _wire->begin();
  
  _wire->beginTransmission(_i2caddr);
  if (_wire->endTransmission())
    return false;   // device not available at the expected address

  reset();
  if (_readRegister8(SI7021_READRHT_REG_CMD) != 0x3A)
    return false;

  readSerialNumber();
  _readRevision();

  return true;
}

/*!
 *  @brief  Reads the humidity value from Si7021 (No Master hold)
 *  @return Returns humidity as float value or NAN when there is error timeout
 */
float Adafruit_Si7021::readHumidity() {
  _wire->beginTransmission(_i2caddr);

  _wire->write(SI7021_MEASRH_NOHOLD_CMD);
  uint8_t err = _wire->endTransmission();

  if (err != 0)
    return NAN; //error

  delay(20);    // account for conversion time for reading humidity

  uint32_t start = millis(); // start timeout
  while(millis()-start < _TRANSACTION_TIMEOUT) {
    if (_wire->requestFrom(_i2caddr, 3) == 3) {
      uint16_t hum = _wire->read() << 8 | _wire->read();
      uint8_t chxsum = _wire->read();

      float humidity = hum;
      humidity *= 125;
      humidity /= 65536;
      humidity -= 6;

      return humidity > 100.0 ? 100.0 : humidity;
    }
    delay(6); // 1/2 typical sample processing time
  }
  return NAN; // Error timeout
}

/*!
 *  @brief  Reads the temperature value from Si7021 (No Master hold)
 *  @return Returns temperature as float value or NAN when there is error timeout
 */
float Adafruit_Si7021::readTemperature() {
  _wire->beginTransmission(_i2caddr);
  _wire->write(SI7021_MEASTEMP_NOHOLD_CMD);
  uint8_t err = _wire->endTransmission();

  if (err != 0)
    return NAN; //error

  delay(20);    // account for conversion time for reading temperature

  uint32_t start = millis(); // start timeout
  while(millis()-start < _TRANSACTION_TIMEOUT) {
    if (_wire->requestFrom(_i2caddr, 3) == 3) {
      uint16_t temp = _wire->read() << 8 | _wire->read();
      uint8_t chxsum = _wire->read();

      float temperature = temp;
      temperature *= 175.72;
      temperature /= 65536;
      temperature -= 46.85;
      return temperature;
    }
    delay(6); // 1/2 typical sample processing time
  }

  return NAN; // Error timeout
}


/*!
 *  @brief  Sends the reset command to Si7021.
 */
void Adafruit_Si7021::reset() {
  _wire->beginTransmission(_i2caddr);
  _wire->write(SI7021_RESET_CMD);
  _wire->endTransmission();
  delay(50);
}

void Adafruit_Si7021::_readRevision(void)
{
    _wire->beginTransmission(_i2caddr);
    _wire->write((uint8_t)(SI7021_FIRMVERS_CMD >> 8));
    _wire->write((uint8_t)(SI7021_FIRMVERS_CMD & 0xFF));
    _wire->endTransmission();
    
    uint32_t start = millis(); // start timeout
    while(millis()-start < _TRANSACTION_TIMEOUT) {
      if (_wire->requestFrom(_i2caddr, 2) == 2) {
        uint8_t rev = _wire->read();
        _wire->read();
    
        if (rev == SI7021_REV_1) {
          rev = 1;
        } else if (rev == SI7021_REV_2) {
          rev = 2;
        }
        _revision = rev;
        return;
      }
      delay(2);
    }
    _revision = 0;
    return; // Error timeout
}

/*!
 *  @brief  Reads serial number and stores It in sernum_a and sernum_b variable
 */
void Adafruit_Si7021::readSerialNumber() {
  _wire->beginTransmission(_i2caddr);
  _wire->write((uint8_t)(SI7021_ID1_CMD >> 8));
  _wire->write((uint8_t)(SI7021_ID1_CMD & 0xFF));
  _wire->endTransmission();

  bool gotData = false;
  uint32_t start = millis(); // start timeout
  while(millis()-start < _TRANSACTION_TIMEOUT) {
    if (_wire->requestFrom(_i2caddr, 8) == 8) {
      gotData = true;
      break;
    }
    delay(2);
  }
  if (!gotData)
    return; // error timeout

  sernum_a = _wire->read();
  _wire->read();
  sernum_a <<= 8;
  sernum_a |= _wire->read();
  _wire->read();
  sernum_a <<= 8;
  sernum_a |= _wire->read();
  _wire->read();
  sernum_a <<= 8;
  sernum_a |= _wire->read();
  _wire->read();

  _wire->beginTransmission(_i2caddr);
  _wire->write((uint8_t)(SI7021_ID2_CMD >> 8));
  _wire->write((uint8_t)(SI7021_ID2_CMD & 0xFF));
  _wire->endTransmission();

  gotData = false;
  start = millis(); // start timeout
  while(millis()-start < _TRANSACTION_TIMEOUT){
    if (_wire->requestFrom(_i2caddr, 8) == 8) {
      gotData = true;
      break;
    }
    delay(2);
  }
  if (!gotData)
    return; // error timeout

  sernum_b = _wire->read();
  _wire->read();
  sernum_b <<= 8;
  sernum_b |= _wire->read();
  _wire->read();
  sernum_b <<= 8;
  sernum_b |= _wire->read();
  _wire->read();
  sernum_b <<= 8;
  sernum_b |= _wire->read();
  _wire->read();

  switch(sernum_b >> 24) {
    case 0:
    case 0xff:
      _model = SI_Engineering_Samples;
        break;
    case 0x0D:
      _model = SI_7013;
      break;
    case 0x14:
      _model = SI_7020;
      break;
    case 0x15:
      _model = SI_7021;
      break;
    default:
      _model = SI_UNKNOWN;
    }    
}

/*!
 *  @brief  Returns sensor model established during init 
 *  @return model value
 */
si_sensorType Adafruit_Si7021::getModel()
{
  return _model;
}

/*******************************************************************/

void Adafruit_Si7021::_writeRegister8(uint8_t reg, uint8_t value) {
  _wire->beginTransmission(_i2caddr);
  _wire->write(reg);
  _wire->write(value);
  _wire->endTransmission();

  //Serial.print("Wrote $"); Serial.print(reg, HEX); Serial.print(": 0x"); Serial.println(value, HEX);
}

uint8_t Adafruit_Si7021::_readRegister8(uint8_t reg) {
  uint8_t value;
  _wire->beginTransmission(_i2caddr);
  _wire->write((uint8_t)reg);
  _wire->endTransmission();

  uint32_t start = millis(); // start timeout
  while(millis()-start < _TRANSACTION_TIMEOUT) {
    if (_wire->requestFrom(_i2caddr, 1) == 1) {
      value = _wire->read();
      return value;
    }
    delay(2);
  }

  return 0; // Error timeout
}

uint16_t Adafruit_Si7021::_readRegister16(uint8_t reg) {
  uint16_t value;
  _wire->beginTransmission(_i2caddr);
  _wire->write(reg);
  _wire->endTransmission(false);

  uint32_t start = millis(); // start timeout
  while(millis()-start < _TRANSACTION_TIMEOUT) {
    if (_wire->requestFrom(_i2caddr, 2) == 2) {
      value = _wire->read() << 8 | _wire->read();
      return value;
    }
    delay(2);
  }
  return 0; // Error timeout
}
