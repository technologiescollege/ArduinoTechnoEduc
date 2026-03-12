/* -------------------------------------------------------------------------- */
/* FILE NAME:   AnalogExpansionCfg.h
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240222
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#ifndef ANALOGEXPANSIONCFG
#define ANALOGEXPANSIONCFG

#include "OptaAnalogProtocol.h"
#include "OptaMsgCommon.h"
#include <cstdint>
#include <cstring>

/* channels maps:
   - first 8 cfgs are channels configuration (ADC, RTD, DAC etc)
   - second 4 cfg are PWM channels configuration 
   - then there is 1 position for RTD timing configuration
   - then there are 8 position for additional ADC configuration
   - then there are 8 position for last value channel output value (DAC)
   - then there is 1 position for the last LED value (all led status)
   - then there are 8 position for default DAC output after timeout
   - then there are 4 position for default PWM value after timeout
   - then there 1 position for the TIMEOUT  */

#define OA_CFG_MSG_NUM  (OA_AN_CHANNELS_NUM +  \
                         OA_PWM_CHANNELS_NUM + \
                         1 +                   \
                         OA_AN_CHANNELS_NUM +  \
                         OA_AN_CHANNELS_NUM +  \
                         1 +                   \
                         OA_AN_CHANNELS_NUM +  \
                         OA_PWM_CHANNELS_NUM + \
                         1)

#define OFFSET_CHANNEL_CONFIG    (0)
#define OFFSET_PWM_CONFIG        (OFFSET_CHANNEL_CONFIG + OA_AN_CHANNELS_NUM)
#define OFFSET_RTD_UPDATE_TIME   (OFFSET_PWM_CONFIG + OA_PWM_CHANNELS_NUM)
#define OFFSET_ADD_ADC_CONFIG    (OFFSET_RTD_UPDATE_TIME + 1)
#define OFFSET_DAC_VALUE         (OFFSET_ADD_ADC_CONFIG + OA_AN_CHANNELS_NUM)
#define OFFSET_LED_VALUE         (OFFSET_DAC_VALUE + OA_AN_CHANNELS_NUM)
#define OFFSET_DAC_DEFAULT_VALUE (OFFSET_LED_VALUE + 1)
#define OFFSET_PWM_DEFAULT_VALUE (OFFSET_DAC_DEFAULT_VALUE + OA_AN_CHANNELS_NUM)
#define OFFSET_TIMEOUT_VALUE     (OFFSET_PWM_DEFAULT_VALUE + OA_PWM_CHANNELS_NUM)
                                
/* this class is used to store the last 'begin' message sent by the controller
 * to an Opta Analog so that it will be possible to quickly "restore" a device
 * configuration if the device is rebooted. There is 1 message for each channel
 * (PWM included) */
class OaChannelCfg {
private:
  int8_t size[OA_CFG_MSG_NUM];
  uint8_t *cfg[OA_CFG_MSG_NUM];
  bool device_is_used = false;

  /* function that allocates the configuration i for a size s
   * it deletes the configuration if already present
   * ALWAYS use the return value --> true means allocation OK */
  bool allocate(uint8_t i, uint8_t s) {
    bool rv = false;
    if (i < OA_CFG_MSG_NUM && s > 0) {
      if (cfg[i] != nullptr) {
        delete[] cfg[i];
        cfg[i] = nullptr;
        size[i] = -1;
      }
      cfg[i] = new uint8_t[s];
      if (cfg[i] != nullptr) {
        rv = true;
      }
    }
    return rv;
  }
  bool is_cfg(uint8_t i) {
    if(i < OA_CFG_MSG_NUM) {
       if (cfg[i] != nullptr && size[i] >= 5) {
         return true;
       }
    }
    return false;
  }

public:
  /* CONSTRUCTOR */
  OaChannelCfg() {
    for (int i = 0; i < OA_CFG_MSG_NUM; i++) {
      size[i] = -1;
      cfg[i] = nullptr;
    }
  }

  bool isExpansionUsed() {
    return device_is_used;
  }

  bool isVoltageAdcCh(uint8_t ch) {
    if (is_cfg(ch)) {
      if (*(cfg[ch] + BP_ARG_POS) == ARG_OA_CH_ADC &&
          *(cfg[ch] + OA_CH_ADC_TYPE_POS) == OA_VOLTAGE_ADC) {
        return true;
      }
    }

    if (is_cfg(ch + OFFSET_ADD_ADC_CONFIG)) {
      if (*(cfg[OFFSET_ADD_ADC_CONFIG + ch] + BP_ARG_POS) == ARG_OA_CH_ADC &&
          *(cfg[OFFSET_ADD_ADC_CONFIG + ch] + OA_CH_ADC_TYPE_POS) ==
              OA_VOLTAGE_ADC) {
        return true;
      }
    }
    return false;
  }

  bool isCurrentAdcCh(uint8_t ch) {
    if (is_cfg(ch)) {
      if (*(cfg[ch] + BP_ARG_POS) == ARG_OA_CH_ADC &&
          *(cfg[ch] + OA_CH_ADC_TYPE_POS) == OA_CURRENT_ADC) {
        return true;
      }
    }

    if (is_cfg(ch + OFFSET_ADD_ADC_CONFIG)) {
      if (*(cfg[OFFSET_ADD_ADC_CONFIG + ch] + BP_ARG_POS) == ARG_OA_CH_ADC &&
          *(cfg[OFFSET_ADD_ADC_CONFIG + ch] + OA_CH_ADC_TYPE_POS) ==
              OA_CURRENT_ADC) {
        return true;
      }
    }
    return false;
  }

  void resetAdditionalAdcCh(uint8_t ch) {
    if (is_cfg(ch + OFFSET_ADD_ADC_CONFIG)) {
      delete[] cfg[ch + OFFSET_ADD_ADC_CONFIG];
      cfg[ch + OFFSET_ADD_ADC_CONFIG] = nullptr;
      size[ch + OFFSET_ADD_ADC_CONFIG] = -1;
    }
  }

  bool isVoltageDacCh(uint8_t ch) {
    if (is_cfg(ch)) {
      if (*(cfg[ch] + BP_ARG_POS) == ARG_OA_CH_DAC &&
          *(cfg[ch] + OA_CH_DAC_TYPE_POS) == OA_VOLTAGE_DAC) {
        return true;
      }
    }
    return false;
  }

  bool isCurrentDacCh(uint8_t ch) {
    if (is_cfg(ch)) {
      if (*(cfg[ch] + BP_ARG_POS) == ARG_OA_CH_DAC &&
          *(cfg[ch] + OA_CH_DAC_TYPE_POS) == OA_CURRENT_DAC) {
        return true;
      }
    }
    return false;
  }

  bool isDigitalInputCh(uint8_t ch) {
    if (is_cfg(ch)) {
      if (*(cfg[ch] + BP_ARG_POS) == ARG_OA_CH_DI) {
        return true;
      }
    }
    return false;
  }

  bool isRtdCh(uint8_t ch) {
    if (is_cfg(ch)) {
      if (*(cfg[ch] + BP_ARG_POS) == ARG_OA_CH_RTD) {
        return true;
      }
    }
    return false;
  }

  bool isRtd3WiresCh(uint8_t ch) {
    if (is_cfg(ch)) {
      if(ch <= 1) {
        if (*(cfg[ch] + BP_ARG_POS) == ARG_OA_CH_RTD && *(cfg[ch] + OA_CH_RTD_3WIRE_POS) == OA_ENABLE) {
          return true;
        }
      }
    }
    return false;
  }

  bool isHighImpedanceCh(uint8_t ch) {
    if (!is_cfg(ch)) {
      return true;
    } else if (is_cfg(ch)) {
      if (*(cfg[ch] + BP_ARG_POS) == ARG_OA_CH_HIGH_IMPEDENCE) {
        return true;
      }
    }
    return false;
  }

  void backup(uint8_t *src, uint8_t ch, uint8_t s) {
    device_is_used = true;
    if (allocate(ch, s)) {
      memcpy(cfg[ch], src, s);
      size[ch] = s;
    }
  }

  /* COPY CONSTRUCTOR */
  OaChannelCfg(const OaChannelCfg &c) {
    for (int i = 0; i < OA_CFG_MSG_NUM; i++) {
      if (c.size[i] > 0 && c.cfg[i] != nullptr) {
        backup(c.cfg[i], i, c.size[i]);
      } else {
        cfg[i] = nullptr;
        size[i] = -1;
      }
    }
  }

  int8_t restore(uint8_t *dst, uint8_t ch) {
    if (ch < OA_CFG_MSG_NUM) {
      if (size[ch] != -1 && cfg[ch] != nullptr) {
        memcpy(dst, cfg[ch], size[ch]);
        return size[ch];
      }
    }
    return 0;
  }

#ifdef DEBUG_ANALOG_CONFIGURRATION
  void print() {
    Serial.println("");
    for(int k = 0; k < OA_CFG_MSG_NUM; k++) {
      Serial.print("Stored configuration " + String(k) + ": " );
      if(size[k] != 0 && cfg[k] != nullptr) {
        for(int i = 0; i < size[k]; i++) {
          Serial.print(cfg[k][i],HEX);
          Serial.print(" ");
        }
        Serial.println();
      }
      else {
        Serial.println("NOT SET!!!");
      }
    }
  }
#endif

  /* DESTRUCTOR */
  ~OaChannelCfg() {
    for (int i = 0; i < OA_CFG_MSG_NUM; i++) {
      if (cfg[i] != nullptr) {
        delete[] cfg[i];
        cfg[i] = nullptr;
      }
    }
  }
};
#endif
