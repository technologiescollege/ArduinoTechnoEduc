/* -------------------------------------------------------------------------- */
/* FILE NAME:   AnalogExpansion.cpp
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240208
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#ifdef ARDUINO_OPTA
#include "AnalogExpansion.h"
#include "OptaController.h"

namespace Opta {

OaChannelCfg AnalogExpansion::cfgs[OPTA_CONTROLLER_MAX_EXPANSION_NUM];

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

AnalogExpansion::AnalogExpansion() {
  iregs[ADD_OA_LED_VALUE] = 0;
  iregs[ADD_FLAG_ADD_ADC_ON_CHANNEL] = 0;
  iregs[ADD_SET_DEFAULT_VALUE_FLAG] = 0;

  for(int i = 0; i < OA_AN_CHANNELS_NUM; i++) {
    iregs[BASE_OA_DAC_ADDRESS + i] = 0;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

Expansion *AnalogExpansion::makeExpansion() { return new AnalogExpansion(); }

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

std::string AnalogExpansion::getProduct() {
  std::string rv(OPTA_ANALOG_DESCRIPTION);
  return rv;
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

AnalogExpansion::~AnalogExpansion() {}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

AnalogExpansion::AnalogExpansion(Expansion &other) {
  AnalogExpansion &ae = (AnalogExpansion &)other;

  type = EXPANSION_NOT_VALID;
  i2c_address = 0;
  ctrl = other.getCtrl();
  index = 255;

  if (other.getType() == EXPANSION_OPTA_ANALOG) {
    iregs = ae.iregs;
    fregs = ae.fregs;
    type = other.getType();
    i2c_address = other.getI2CAddress();
    ctrl = other.getCtrl();
    index = other.getIndex();
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void AnalogExpansion::startUp(Controller *ptr) {

  if (ptr == nullptr) {
    return;
  }

  for (int i = 0; i < OPTA_CONTROLLER_MAX_EXPANSION_NUM; i++) {
    AnalogExpansion exp = ptr->getExpansion(i);
    if (exp) {
      if(AnalogExpansion::cfgs[i].isExpansionUsed()) {
        for (int k = 0; k < OA_CFG_MSG_NUM; k++) {
          uint8_t tx_bytes = AnalogExpansion::cfgs[i].restore(ptr->getTxBuffer(), k);
          if (tx_bytes) {
            ptr->send(exp.getI2CAddress(), exp.getIndex(), exp.getType(),
                      tx_bytes, getExpectedAnsLen(ANS_LEN_OA_ACK));
            /* channel configuration takes some times on the expansion side*/
            delay(50);
          }
        }
        exp.updateAnalogOutputs();
      }
      else {
        for (int ch = 0; ch < OA_AN_CHANNELS_NUM; ch++) {
          exp.beginChannelAsHighImpedance(ch);
          exp.setDefaultDac(ch,0);
        }
        for (int ch = OA_PWM_CH_FIRST; ch <= OA_PWM_CH_LAST; ch++) {
          exp.setPwm(ch,0,0);
          exp.setDefaultPwm(ch,0,0);
        }
        for (int ch = OA_LED_1; ch < OA_LED_NUM; ch++) {
          exp.switchLedOff(ch,false);
        }
        exp.updateLeds();
        exp.beginRtdUpdateTime(1000);
        exp.setTimeoutForDefaultValues(0xFFFF);
  
      }
    }
  }
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t AnalogExpansion::msg_begin_adc() {
  if( iregs[ADD_OA_PIN] >= OA_AN_CHANNELS_NUM && 
      index >= OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    return 0;
  }

  if (ctrl != nullptr) {
    if (adc_registers_defined()) {
      
      ctrl->setTx(iregs[ADD_OA_PIN], OA_CH_ADC_CHANNEL_POS);
      ctrl->setTx(iregs[ADD_OA_ADC_TYPE], OA_CH_ADC_TYPE_POS);
      ctrl->setTx(iregs[ADD_OA_ADC_USE_PULL_DOWN], OA_CH_ADC_PULL_DOWN_POS);
      ctrl->setTx(iregs[ADD_OA_ADC_USE_REJECTION], OA_CH_ADC_REJECTION_POS);
      ctrl->setTx(iregs[ADD_OA_ADC_USE_DIAGNOSTIC], OA_CH_ADC_DIAGNOSTIC_POS);
      ctrl->setTx(iregs[ADD_OA_ADC_MOVE_AVERAGE], OA_CH_ADC_MOVING_AVE_POS);
      int offset_add_adc_messages = OFFSET_CHANNEL_CONFIG;
      if (iregs[ADD_FLAG_ADD_ADC_ON_CHANNEL] == 1) {
        ctrl->setTx(OA_ENABLE, OA_CH_ADC_ADDING_ADC_POS);
        offset_add_adc_messages = OFFSET_ADD_ADC_CONFIG;
      } else {
        ctrl->setTx(OA_DISABLE, OA_CH_ADC_ADDING_ADC_POS);
      }
      iregs[ADD_FLAG_ADD_ADC_ON_CHANNEL] = 0;
      uint8_t rv = prepareSetMsg(ctrl->getTxBuffer(), ARG_OA_CH_ADC,
                                 LEN_OA_CH_ADC);
      AnalogExpansion::cfgs[index].resetAdditionalAdcCh(iregs[ADD_OA_PIN]);
      AnalogExpansion::cfgs[index].backup(ctrl->getTxBuffer(),
                           iregs[ADD_OA_PIN] + offset_add_adc_messages, rv);
      return rv;
    }
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t AnalogExpansion::msg_begin_di() {
  if( iregs[ADD_OA_PIN] >= OA_AN_CHANNELS_NUM && 
      index >= OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    return 0;
  }

  if (ctrl != nullptr) {
    if (di_registers_defined()) {
      ctrl->setTx(iregs[ADD_OA_PIN], OA_CH_DI_CHANNEL_POS);
      ctrl->setTx(iregs[ADD_OA_DI_USE_FILTER], OA_CH_DI_FILTER_COMP_POS);
      ctrl->setTx(iregs[ADD_OA_DI_INVERT], OA_CH_DI_INVERT_COMP_POS);
      ctrl->setTx(OA_ENABLE, OA_CH_DI_ENABLE_COMP_POS);
      ctrl->setTx(iregs[ADD_OA_DI_SIMPLE_DEB], OA_CH_DI_DEBOUNCE_SIMPLE_POS);
      ctrl->setTx(iregs[ADD_OA_DI_SCALE_TH_WITH_VCC], OA_CH_DI_SCALE_COMP_POS);
      ctrl->setTx(iregs[ADD_OA_DI_THRESHOLD], OA_CH_DI_COMP_TH_POS);
      ctrl->setTx(iregs[ADD_OA_DI_SINK_CURRENT], OA_CH_DI_CURR_SINK_POS);
      ctrl->setTx(iregs[ADD_OA_DI_DEB_TIME], OA_CH_DI_DEBOUNCE_TIME_POS);

      uint8_t rv = prepareSetMsg(ctrl->getTxBuffer(), ARG_OA_CH_DI,
                                 LEN_OA_CH_DI);
      
      AnalogExpansion::cfgs[index].resetAdditionalAdcCh(iregs[ADD_OA_PIN]);
      AnalogExpansion::cfgs[index].backup(ctrl->getTxBuffer(), 
                         iregs[ADD_OA_PIN] + OFFSET_CHANNEL_CONFIG,
                         rv);
      return rv;
    }
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool AnalogExpansion::di_registers_defined() {
  return (
      addressExist(ADD_OA_PIN) && addressExist(ADD_OA_DI_USE_FILTER) &&
      addressExist(ADD_OA_DI_INVERT) && addressExist(ADD_OA_DI_SINK_CURRENT) &&
      addressExist(ADD_OA_DI_DEB_TIME) && addressExist(ADD_OA_DI_SIMPLE_DEB) &&
      addressExist(ADD_OA_DI_SCALE_TH_WITH_VCC) &&
      addressExist(ADD_OA_DI_THRESHOLD));
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool AnalogExpansion::adc_registers_defined() {
  return (addressExist(ADD_OA_PIN) && addressExist(ADD_OA_ADC_TYPE) &&
          addressExist(ADD_OA_ADC_USE_PULL_DOWN) &&
          addressExist(ADD_OA_ADC_USE_REJECTION) &&
          addressExist(ADD_OA_ADC_USE_DIAGNOSTIC) &&
          addressExist(ADD_OA_ADC_MOVE_AVERAGE));
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::beginChannelAsAdc(uint8_t ch, OaAdcType_t type,
                                        bool pull_down, bool rejection,
                                        bool diagnostic, uint8_t ma) {

  iregs[ADD_OA_PIN] = ch;
  iregs[ADD_OA_ADC_TYPE] = type;
  iregs[ADD_OA_ADC_USE_PULL_DOWN] = (pull_down) ? OA_ENABLE : OA_DISABLE;
  iregs[ADD_OA_ADC_USE_REJECTION] = (rejection) ? OA_ENABLE : OA_DISABLE;
  iregs[ADD_OA_ADC_USE_DIAGNOSTIC] = (diagnostic) ? OA_ENABLE : OA_DISABLE;
  iregs[ADD_OA_ADC_MOVE_AVERAGE] = ma;

#ifdef DEBUG_BEGIN_ADC
  unsigned int err = execute(BEGIN_CHANNEL_AS_ADC);
  Serial.println("BEGIN CHANNEL AS ADC err = " + String(err));
#else
  execute(BEGIN_CHANNEL_AS_ADC);
#endif

  iregs.erase(ADD_OA_PIN);
  iregs.erase(ADD_OA_ADC_TYPE);
  iregs.erase(ADD_OA_ADC_USE_PULL_DOWN);
  iregs.erase(ADD_OA_ADC_USE_REJECTION);
  iregs.erase(ADD_OA_ADC_USE_DIAGNOSTIC);
  iregs.erase(ADD_OA_ADC_MOVE_AVERAGE);
}

void AnalogExpansion::addAdcOnChannel(uint8_t ch, OaAdcType_t type,
                                      bool pull_down, bool rejection,
                                      bool diagnostic, uint8_t ma) {
  if (isChVoltageAdc(ch) || isChCurrentAdc(ch) || isChRtd(ch)) {
    return;
  }

  iregs[ADD_FLAG_ADD_ADC_ON_CHANNEL] = 1;
  beginChannelAsAdc(ch, type, pull_down, rejection, diagnostic, ma);
}
void AnalogExpansion::addVoltageAdcOnChannel(uint8_t ch) {
  addAdcOnChannel(ch, OA_VOLTAGE_ADC, true, true, false, 0);
}
void AnalogExpansion::addCurrentAdcOnChannel(uint8_t ch) {
  addAdcOnChannel(ch, OA_CURRENT_ADC, false, true, false, 0);
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::beginChannelAsVoltageAdc(uint8_t ch) {
  beginChannelAsAdc(ch, OA_VOLTAGE_ADC, true, true, false, 0);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::beginChannelAsCurrentAdc(uint8_t ch) {
  beginChannelAsAdc(ch, OA_CURRENT_ADC, false, true, false, 0);
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::beginChannelAsDigitalInput(uint8_t ch, bool filter,
                                                 bool invert, bool simple_deb,
                                                 OaDiSinkCurrent_t sink_cur,
                                                 OaDebounceTime_t deb_time,
                                                 bool scale, float th,
                                                 float Vcc) {
  iregs[ADD_OA_PIN] = ch;
  iregs[ADD_OA_DI_USE_FILTER] = (filter) ? OA_ENABLE : OA_DISABLE;
  iregs[ADD_OA_DI_INVERT] = (invert) ? OA_ENABLE : OA_DISABLE;
  iregs[ADD_OA_DI_SINK_CURRENT] = sink_cur;
  iregs[ADD_OA_DI_DEB_TIME] = deb_time;
  iregs[ADD_OA_DI_SIMPLE_DEB] = (simple_deb) ? OA_ENABLE : OA_DISABLE;
  iregs[ADD_OA_DI_SCALE_TH_WITH_VCC] = (scale) ? OA_ENABLE : OA_DISABLE;

  if (th < 0) {
    th = 0;
  }
  float code = 15.0;
  /* the code to be written depends on the scale parameter (see AD74412R
   * datasheet for details)*/
  if (scale) {
    if (th > Vcc) {
      th = Vcc;
    }
    code = (30.0 * th / Vcc) - 0.5;
  } else {
    if (th > 16.0) {
      th = 16.0;
    }
    code = 2.0 * th - 1;
  }
  iregs[ADD_OA_DI_THRESHOLD] = (uint8_t)code;

  /*unsigned int err = */ execute(BEGIN_CHANNEL_AS_DI);
  /* Serial.println("begin digital input err = " + String(err)); */

  iregs.erase(ADD_OA_PIN);
  iregs.erase(ADD_OA_DI_USE_FILTER);
  iregs.erase(ADD_OA_DI_INVERT);
  iregs.erase(ADD_OA_DI_SINK_CURRENT);
  iregs.erase(ADD_OA_DI_DEB_TIME);
  iregs.erase(ADD_OA_DI_SIMPLE_DEB);
  iregs.erase(ADD_OA_DI_SCALE_TH_WITH_VCC);
  iregs.erase(ADD_OA_DI_THRESHOLD);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::beginChannelAsDigitalInput(uint8_t ch) {
  beginChannelAsDigitalInput(ch, true, false, true, OA_DI_SINK_1,
                             OA_DI_DEB_TIME_24, false, 5.2, 16.0);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::beginChannelAsDac(uint8_t ch, OaDacType_t type,
                                        bool limit_current, bool enable_slew,
                                        OaDacSlewRate_t sr) {
  iregs[ADD_OA_PIN] = ch;
  iregs[ADD_OA_DAC_TYPE] = type;
  iregs[ADD_OA_DAC_LIMIT_CURRENT] = (limit_current) ? OA_ENABLE : OA_DISABLE;
  iregs[ADD_OA_DAC_USE_SLEW] = (enable_slew) ? OA_ENABLE : OA_DISABLE;
  iregs[ADD_OA_DAC_SLEW_RATE] = sr;

  iregs[BASE_OA_DAC_ADDRESS + ch] = 0;

  execute(BEGIN_CHANNEL_AS_DAC);

  iregs.erase(ADD_OA_PIN);
  iregs.erase(ADD_OA_DAC_TYPE);
  iregs.erase(ADD_OA_DAC_LIMIT_CURRENT);
  iregs.erase(ADD_OA_DAC_USE_SLEW);
  iregs.erase(ADD_OA_DAC_SLEW_RATE);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::beginChannelAsVoltageDac(uint8_t ch) {
  beginChannelAsDac(ch, OA_VOLTAGE_DAC, true, false, OA_SLEW_RATE_0);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::beginChannelAsCurrentDac(uint8_t ch) {
  beginChannelAsDac(ch, OA_CURRENT_DAC, false, false, OA_SLEW_RATE_0);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t AnalogExpansion::msg_begin_dac() {
  if( iregs[ADD_OA_PIN] >= OA_AN_CHANNELS_NUM && 
    index >= OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    return 0;
  }

  if (ctrl != nullptr) {
    if (dac_registers_defined()) {
      ctrl->setTx(iregs[ADD_OA_PIN], OA_CH_DAC_CHANNEL_POS);
      ctrl->setTx(iregs[ADD_OA_DAC_TYPE], OA_CH_DAC_TYPE_POS);
      ctrl->setTx(iregs[ADD_OA_DAC_LIMIT_CURRENT], OA_CH_DAC_LIMIT_CURRENT_POS);
      ctrl->setTx(iregs[ADD_OA_DAC_USE_SLEW], OA_CH_DAC_ENABLE_SLEW_POS);
      ctrl->setTx(iregs[ADD_OA_DAC_SLEW_RATE], OA_CH_DAC_SLEW_RATE_POS);

      uint8_t rv = prepareSetMsg(ctrl->getTxBuffer(), ARG_OA_CH_DAC,
                                 LEN_OA_CH_DAC);
      
      AnalogExpansion::cfgs[index].resetAdditionalAdcCh(iregs[ADD_OA_PIN]);
      AnalogExpansion::cfgs[index].backup(ctrl->getTxBuffer(), 
                         iregs[ADD_OA_PIN] + OFFSET_CHANNEL_CONFIG, 
                         rv);
      
      return rv;
    }
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t AnalogExpansion::msg_begin_high_imp() {
  if( iregs[ADD_OA_PIN] >= OA_AN_CHANNELS_NUM && 
    index >= OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    return 0;
  }

  if (ctrl != nullptr) {

    ctrl->setTx(iregs[ADD_OA_PIN], OA_HIGH_IMPEDENCE_CH_POS);
    uint8_t rv =
        prepareSetMsg(ctrl->getTxBuffer(), ARG_OA_CH_HIGH_IMPEDENCE,
                      LEN_OA_CH_HIGH_IMPEDENCE);
    if (index < OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
      AnalogExpansion::cfgs[index].resetAdditionalAdcCh(iregs[ADD_OA_PIN]);
      AnalogExpansion::cfgs[index].backup(ctrl->getTxBuffer(), 
                         iregs[ADD_OA_PIN] + OFFSET_CHANNEL_CONFIG, 
                         rv);
    }
    return rv;
  }
  return 0;
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool AnalogExpansion::dac_registers_defined() {
  return (addressExist(ADD_OA_PIN) && addressExist(ADD_OA_DAC_TYPE) &&
          addressExist(ADD_OA_DAC_LIMIT_CURRENT) &&
          addressExist(ADD_OA_DAC_USE_SLEW) &&
          addressExist(ADD_OA_DAC_SLEW_RATE));
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::beginChannelAsHighImpedance(uint8_t ch) {
  iregs[ADD_OA_PIN] = ch;
  execute(BEGIN_CHANNEL_AS_HIGH_IMP);
}

void AnalogExpansion::beginChannelAsRtd(uint8_t ch, bool use_3_wires,
                                        float current) {
  iregs[ADD_OA_PIN] = ch;
  iregs[ADD_OA_RTD_USE_3_WIRES] = (use_3_wires) ? OA_ENABLE : OA_DISABLE;
  fregs[ADD_OA_RTD_CURRENT] = current;

  /*int32_t err =*/execute(BEGIN_CHANNEL_AS_RTD);
  /* Serial.println("Execute BEGIN as CHANNEL as RTD " + String(err));*/

  iregs.erase(ADD_OA_PIN);
  iregs.erase(ADD_OA_RTD_USE_3_WIRES);
  fregs.erase(ADD_OA_RTD_CURRENT);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::beginRtdUpdateTime(uint16_t time) {
  iregs[ADD_OA_RTD_TIME] = time;
  execute(SEND_TIMING);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::setTimeoutForDefaultValues(uint16_t timeout_ms) {
  iregs[ADD_OA_TIMEOUT_ADDRESS] = timeout_ms;
  iregs[ADD_SET_DEFAULT_VALUE_FLAG] = 1;
  execute(SEND_TIMING);
  
}


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t AnalogExpansion::msg_send_time() {
  if( index >= OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    return 0;
  }
  uint32_t time_index = ADD_OA_RTD_TIME;
  uint8_t msg_argument = ARG_OA_SET_RTD_UPDATE_TIME;
  uint8_t offset = OFFSET_RTD_UPDATE_TIME;
  if(iregs[ADD_SET_DEFAULT_VALUE_FLAG]) {
    iregs[ADD_SET_DEFAULT_VALUE_FLAG] = 0;
    msg_argument = ARG_OA_SET_TIMEOUT_TIME;
    time_index = ADD_OA_TIMEOUT_ADDRESS;
    offset = OFFSET_TIMEOUT_VALUE;
  }

  if (ctrl != nullptr) {
    if (addressExist(time_index)) {
      ctrl->setTx((uint8_t)(iregs[time_index] & 0xFF),
                  OA_SET_RTD_UPDATE_TIME_POS);
      ctrl->setTx((uint8_t)((iregs[time_index] & 0xFF00) >> 8),
                  OA_SET_RTD_UPDATE_TIME_POS + 1);
      
      uint8_t rv =  prepareSetMsg(ctrl->getTxBuffer(), msg_argument,
                        LEN_OA_SET_RTD_UPDATE_TIME);
      cfgs[index].backup(ctrl->getTxBuffer(), offset, rv);

      return rv;

    }
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t AnalogExpansion::msg_begin_rtd() {
  if( iregs[ADD_OA_PIN] >= OA_AN_CHANNELS_NUM && 
    index >= OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    return 0;
  }

  if (ctrl != nullptr) {
    if (rtd_registers_defined()) {
      ctrl->setTx(iregs[ADD_OA_PIN], OA_CH_RTD_CHANNEL_POS);
      ctrl->setTx(iregs[ADD_OA_RTD_USE_3_WIRES], OA_CH_RTD_3WIRE_POS);

      Float_u v;
      v.value =
          (fregs[ADD_OA_RTD_CURRENT] < 25.0) ? fregs[ADD_OA_RTD_CURRENT] : 25.0;
      for (int i = 0; i < 4; i++) {
        ctrl->setTx(v.bytes[i], OA_CH_RTD_CURRENT_POS + i);
      }

      uint8_t rv = prepareSetMsg(ctrl->getTxBuffer(), ARG_OA_CH_RTD,
                                 LEN_OA_CH_RTD);
      
      cfgs[index].resetAdditionalAdcCh(iregs[ADD_OA_PIN]);
      cfgs[index].backup(ctrl->getTxBuffer(), 
                         iregs[ADD_OA_PIN] + OFFSET_CHANNEL_CONFIG, 
                         rv);
      return rv;
    }
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool AnalogExpansion::rtd_registers_defined() {
  return (addressExist(ADD_OA_PIN) && addressExist(ADD_OA_RTD_USE_3_WIRES) &&
          addressFloatExist(ADD_OA_RTD_CURRENT));
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::setPwm(uint8_t ch, uint32_t period, uint32_t pulse) {
  /* sanity checks to verify input function parameter */
  if (ch < OA_FIRST_PWM_CH || ch > OA_LAST_PWM_CH) {
    return;
  }
  /* period == 0 must "pass" because is the condition to stop the PWM*/
  if (period != 0) {
    if (pulse >= period) {
      return;
    }
  }

  iregs[ADD_OA_PIN] = ch - OA_FIRST_PWM_CH;
  uint32_t per_add = BASE_OA_PWM_ADDRESS + iregs[ADD_OA_PIN];
  uint32_t pul_add =
      BASE_OA_PWM_ADDRESS + iregs[ADD_OA_PIN] + OA_PWM_CHANNELS_NUM;

  /* if the address is not defined --> the value has never been sent
   * to the expansion => send it*/
  if (!addressExist(per_add) || !addressExist(pul_add) || iregs[ADD_SET_DEFAULT_VALUE_FLAG] == 1) {
    iregs[per_add] = period;
    iregs[pul_add] = pulse;
    execute(SET_PWM);
  }
  /* if the address is defined and the value different to the one held
   * by the correspondent register => send it */
  else if (period != iregs[per_add] || pulse != iregs[pul_add]) {
    iregs[per_add] = period;
    iregs[pul_add] = pulse;
    execute(SET_PWM);
  }
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::setDefaultPwm(uint8_t ch, uint32_t period, uint32_t pulse) {
  iregs[ADD_SET_DEFAULT_VALUE_FLAG] = 1;
  setPwm(ch, period, pulse);
}



/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* get Pwm period in micro seconds */
  uint32_t AnalogExpansion::getPwmPeriod(uint8_t ch) {
    if (ch >= OA_FIRST_PWM_CH && ch <= OA_LAST_PWM_CH) {
      ch = ch - OA_FIRST_PWM_CH;
    }
    else if(ch >= OA_PWM_CHANNELS_NUM) {
      return 0;
    }
    
    uint32_t per_add = BASE_OA_PWM_ADDRESS + ch;
    if (!addressExist(per_add)) {
      return 0;
    }
    else {
      return iregs[per_add];
    }
  }

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

  /* get Pwm pulse in micro seconds */
  uint32_t AnalogExpansion::getPwmPulse(uint8_t ch) {
    if (ch >= OA_FIRST_PWM_CH && ch <= OA_LAST_PWM_CH) {
      ch = ch - OA_FIRST_PWM_CH;
    }
    else if(ch >= OA_PWM_CHANNELS_NUM) {
      return 0;
    }
    
    uint32_t pul_add = BASE_OA_PWM_ADDRESS + ch + OA_PWM_CHANNELS_NUM;
    if (!addressExist(pul_add)) {
      return 0;
    }
    else {
      return iregs[pul_add];
    }
  }

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

  float AnalogExpansion::getPwmFreqHz(uint8_t ch) {
    float period = (float)getPwmPeriod(ch);
    if(period > 0) {
      period = period / 1000000; //get seconds 
      return 1 / period;
    }
    return 0.0;

  }

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

  float AnalogExpansion::getPwmPulsePerc(uint8_t ch) {
    float period = (float)getPwmPeriod(ch);
    float pulse = (float)getPwmPulse(ch);
    if(period > 0 && pulse <= period) {
      return pulse * 100.0 / period;
    }
    return 0.0;

  }

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t AnalogExpansion::msg_set_pwm() {
  if (ctrl != nullptr) {
    uint32_t per_add = BASE_OA_PWM_ADDRESS + iregs[ADD_OA_PIN];
    uint32_t pul_add =
        BASE_OA_PWM_ADDRESS + iregs[ADD_OA_PIN] + OA_PWM_CHANNELS_NUM;
    if (addressExist(ADD_OA_PIN) && addressExist(per_add) &&
        addressExist(pul_add)) {
      ctrl->setTx(iregs[ADD_OA_PIN], OA_CH_RTD_CHANNEL_POS);

      ctrl->setTx((uint8_t)(iregs[per_add] & 0xFF), OA_SET_PWM_PERIOD_POS);
      ctrl->setTx((uint8_t)((iregs[per_add] & 0xFF00) >> 8),
                  OA_SET_PWM_PERIOD_POS + 1);
      ctrl->setTx((uint8_t)((iregs[per_add] & 0xFF0000) >> 16),
                  OA_SET_PWM_PERIOD_POS + 2);
      ctrl->setTx((uint8_t)((iregs[per_add] & 0xFF000000) >> 24),
                  OA_SET_PWM_PERIOD_POS + 3);

      ctrl->setTx((uint8_t)(iregs[pul_add] & 0xFF), OA_SET_PWM_PULSE_POS);
      ctrl->setTx((uint8_t)((iregs[pul_add] & 0xFF00) >> 8),
                  OA_SET_PWM_PULSE_POS + 1);
      ctrl->setTx((uint8_t)((iregs[pul_add] & 0xFF0000) >> 16),
                  OA_SET_PWM_PULSE_POS + 2);
      ctrl->setTx((uint8_t)((iregs[pul_add] & 0xFF000000) >> 24),
                  OA_SET_PWM_PULSE_POS + 3);

      uint8_t msg_argument = ARG_OA_SET_PWM;
      uint32_t offset_backup = OFFSET_PWM_CONFIG; 

      if(iregs[ADD_SET_DEFAULT_VALUE_FLAG] == 1) {
        iregs[ADD_SET_DEFAULT_VALUE_FLAG] = 0;
        msg_argument = ARD_OA_SET_DEFAULT_PWM; 
        offset_backup = OFFSET_PWM_DEFAULT_VALUE;
      }
      uint8_t rv = prepareSetMsg(ctrl->getTxBuffer(), msg_argument,LEN_OA_SET_PWM);
      if (index < OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
        cfgs[index].backup(ctrl->getTxBuffer(), iregs[ADD_OA_PIN] + offset_backup, rv);
      }
      return rv;
    }
  }
  return 0;
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint16_t AnalogExpansion::getAdc(uint8_t ch, bool update /* true */) {
  if (ch >= OA_AN_CHANNELS_NUM) {
    return 0;
  }
  iregs[ADD_OA_PIN] = ch;
  if (update) {
    /*uint32_t err =*/execute(GET_SINGLE_ANALOG_INPUT);
    /*Serial.println("err = " + String(err));*/
  }
  return iregs[BASE_OA_ADC_ADDRESS + ch];
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t AnalogExpansion::msg_get_adc() {
  if (ctrl != nullptr) {
    if (addressExist(ADD_OA_PIN)) {
      ctrl->setTx(iregs[ADD_OA_PIN], OA_CH_ADC_CHANNEL_POS);

      return prepareGetMsg(ctrl->getTxBuffer(), ARG_OA_GET_ADC,LEN_OA_GET_ADC);
    }
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool AnalogExpansion::parse_ans_get_adc() {
  if (ctrl != nullptr) {
    if (checkAnsGetReceived(ctrl->getRxBuffer(), ANS_ARG_OA_GET_ADC,
                            ANS_LEN_OA_GET_ADC)) {
      uint32_t ch = ctrl->getRx(ANS_OA_ADC_CHANNEL_POS);
      if (ch < OA_AN_CHANNELS_NUM) {
        iregs[BASE_OA_ADC_ADDRESS + ch] = ctrl->getRx(ANS_OA_ADC_VALUE_POS);
        iregs[BASE_OA_ADC_ADDRESS + ch] +=
            (ctrl->getRx(ANS_OA_ADC_VALUE_POS + 1) << 8);
      }
      return true;
    }
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

float AnalogExpansion::pinVoltage(uint8_t ch, bool update /*= true*/) {
  if(cfgs[index].isVoltageDacCh(ch)) {
    unsigned int output_value = iregs[BASE_OA_DAC_ADDRESS + ch];
    if(output_value > 8191) {
      output_value = 8191;
    }
    return (float)output_value * 11.0 / 8191.0; 
  }
  else if (cfgs[index].isVoltageAdcCh(ch)) {
    uint16_t v = getAdc(ch, update);
    return 10.0 * (float)v / 65535.0;
  }
  return -1.0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

float AnalogExpansion::pinCurrent(uint8_t ch, bool update /*= true*/) {
  if(cfgs[index].isCurrentDacCh(ch)) {
    unsigned int output_value = iregs[BASE_OA_DAC_ADDRESS + ch];
    if(output_value > 8191) {
      output_value = 8191;
    }
    float rv = (float)output_value * 25.0 / 8191.0;
    return rv;
  }
  else if (cfgs[index].isVoltageDacCh(ch) && cfgs[index].isCurrentAdcCh(ch)) {
    float code = (float)getAdc(ch, update);
    float current = (code / 65535.0) * 5.0;
    current = current - 2.5;
    current = current / 100.0;
    return current * 1000.0; // convert to mA
  } else if (cfgs[index].isCurrentAdcCh(ch)) {
    uint16_t v = getAdc(ch, update);
    return 25.0 * (float)v / 65535.0;
  }
  return -1.0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::setDac(uint8_t ch, uint16_t value,
                             bool update /* = true*/) {
  if (ch < OA_AN_CHANNELS_NUM) {
    iregs[ADD_OA_PIN] = ch;
    if (value > 8191) {
      value = 8191;
    }
    /* if the address is not defined --> the value has never been sent
     * to the expansion => send it*/
    iregs[BASE_OA_DAC_ADDRESS + ch] = value;
    iregs[ADD_UPDATE_ANALOG_OUTPUT] = (update) ? 1 : 0;
    unsigned int err = 1;
    uint8_t t = 0;
    do {
      err = execute(SET_SINGLE_ANALOG_OUTPUT);
      if(err) {
        delay(5);
      }
      t++;
    } while(err != EXECUTE_OK && t < 3);

  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::setDefaultDac(uint8_t ch, uint16_t value) {
  iregs[ADD_SET_DEFAULT_VALUE_FLAG] = 1;
  setDac(ch, value, false);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool AnalogExpansion::setDefaultPinVoltage(uint8_t ch, float voltage ) {
  if(ch < OA_AN_CHANNELS_NUM) {
    if(cfgs[index].isVoltageDacCh(ch)) {
      if (voltage > 11.0) {
        voltage = 11.0;
      }
      float v = 8191.0 * voltage / 11.0;
      setDefaultDac(ch, (uint16_t)v);
      return true;
    }
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool AnalogExpansion::setDefaultPinCurrent(uint8_t ch, float current) {
  if(ch < OA_AN_CHANNELS_NUM) {
    if(cfgs[index].isCurrentDacCh(ch)) {
      if (current > 25.0) {
        current = 25.0;
      }
      float v = 8191.0 * current / 25.0;
      setDefaultDac(ch, (uint16_t)v);
      return true;
    }
  }
  return false;

}


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t AnalogExpansion::msg_set_dac() {
  if( iregs[ADD_OA_PIN] >= OA_AN_CHANNELS_NUM && 
    index >= OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    return 0;
  }

  if (ctrl != nullptr) {
    if (addressExist(ADD_OA_PIN) &&
        addressExist(BASE_OA_DAC_ADDRESS + iregs[ADD_OA_PIN])) {
      ctrl->setTx(iregs[ADD_OA_PIN], OA_SET_DAC_CHANNEL_POS);
      ctrl->setTx(
          (uint8_t)(iregs[BASE_OA_DAC_ADDRESS + iregs[ADD_OA_PIN]] & 0xFF),
          OA_SET_DAC_VALUE_POS);
      ctrl->setTx(
          (uint8_t)((iregs[BASE_OA_DAC_ADDRESS + iregs[ADD_OA_PIN]] & 0xFF00) >>
                    8),
          OA_SET_DAC_VALUE_POS + 1);

      ctrl->setTx(iregs[ADD_UPDATE_ANALOG_OUTPUT], OA_SET_DAC_UPDATE_VALUE);

      uint8_t msg_argument = ARG_OA_SET_DAC;
      uint8_t backup_offset = OFFSET_DAC_VALUE;
      if(iregs[ADD_SET_DEFAULT_VALUE_FLAG]) {
        iregs[ADD_SET_DEFAULT_VALUE_FLAG] = 0;
        msg_argument = ARG_OA_SET_DAC_DEFAULT;
        backup_offset = OFFSET_DAC_DEFAULT_VALUE;
      }

      uint8_t rv = prepareSetMsg(ctrl->getTxBuffer(), msg_argument,LEN_OA_SET_DAC);
      AnalogExpansion::cfgs[index].backup(ctrl->getTxBuffer(), 
                         iregs[ADD_OA_PIN] + backup_offset, 
                         rv);

      return rv;
    }
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t AnalogExpansion::msg_set_all_dac() {
  if (ctrl != nullptr) {
    return prepareSetMsg(ctrl->getTxBuffer(), ARG_OA_SET_ALL_DAC,LEN_OA_SET_ALL_DAC);
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::updateAnalogOutputs() { execute(SET_ALL_ANALOG_OUTPUTS); }

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void AnalogExpansion::pinVoltage(uint8_t ch, float voltage,
                                 bool update /*= true*/) {
  if(cfgs[index].isVoltageDacCh(ch)) {
    if (voltage > 11.0) {
      voltage = 11.0;
    }
    float v = 8191.0 * voltage / 11.0;
    setDac(ch, (uint16_t)v, update);
  }
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void AnalogExpansion::pinCurrent(uint8_t ch, float current,
                                 bool update /* = true*/) {
  if(cfgs[index].isCurrentDacCh(ch)) {
    if (current > 25.0) {
      current = 25.0;
    }
    float v = 8191.0 * current / 25.0;
    setDac(ch, (uint16_t)v, update);
  }
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
float AnalogExpansion::getRtd(uint8_t ch) {
  if (ch >= OA_AN_CHANNELS_NUM) {
    return 0;
  }
  iregs[ADD_OA_PIN] = ch;
  /*uint32_t err = */ execute(GET_RTD);
  /*Serial.println("GET RTD err = " + String(err));*/
  if (fregs[BASE_OA_RTD_ADDRESS + ch] <= 0) {
    return -1.0;
  }
  return fregs[BASE_OA_RTD_ADDRESS + ch];
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t AnalogExpansion::msg_get_rtd() {
  if (ctrl != nullptr) {
    if (addressExist(ADD_OA_PIN)) {
      ctrl->setTx(iregs[ADD_OA_PIN], OA_GET_RTD_CHANNEL_POS);

      return prepareGetMsg(ctrl->getTxBuffer(), ARG_OA_GET_RTD, LEN_OA_GET_RTD);
    }
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool AnalogExpansion::parse_ans_get_rtd() {
  if (ctrl != nullptr) {
    if (checkAnsGetReceived(ctrl->getRxBuffer(), ANS_ARG_OA_GET_RTD,
                            ANS_LEN_OA_GET_RTD)) {
      Float_u v;
      for (int i = 0; i < 4; i++) {
        v.bytes[i] = ctrl->getRx(ANS_OA_GET_RTD_VALUE_POS + i);
      }
      uint8_t ch = ctrl->getRx(ANS_OA_GET_RTD_CHANNEL_POS);
      fregs[BASE_OA_RTD_ADDRESS + ch] = v.value;
      return true;
    }
  }
  return false;
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void AnalogExpansion::switchLedOn(uint8_t pin, bool update /* = true */) {
  if (pin < OA_LED_NUM) {
    write(BASE_OA_LED_ADDRESS + pin, 1);
    if (update) {
      updateLeds();
    }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::switchLedOff(uint8_t pin, bool update /* = true */) {
  if (pin < OA_LED_NUM) {
    write(BASE_OA_LED_ADDRESS + pin, 0);
    if (update) {
      updateLeds();
    }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::updateLeds() {
  /*uint32_t err =*/execute(SET_LED);
  /*Serial.println("SET LED RESULT = " + String(err));*/
}


uint8_t AnalogExpansion::msg_get_ch_function() {
  if( iregs[ADD_OA_PIN] >= OA_AN_CHANNELS_NUM) {
    return 0;
  }
  if (ctrl != nullptr) {
   
      ctrl->setTx(iregs[ADD_OA_PIN], GET_CHANNEL_FUNCTION_CH_POS);
      uint8_t rv = prepareGetMsg(ctrl->getTxBuffer(), ARG_GET_CHANNEL_FUNCTION,
                                 LEN_GET_CHANNEL_FUNCTION);

      return rv;
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

CfgFun_t AnalogExpansion::get_channel_function(uint8_t ch) {
  iregs[ADD_OA_PIN] = ch;
  execute(GET_CHANNEL_FUNCTION);
  if(ch == iregs[ADD_OA_PIN_OUTPUT]) {
    return (CfgFun_t)iregs[ADD_CH_FUNCTION];
  }
  return CH_FUNC_UNDEFINED;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool AnalogExpansion::parse_get_ch_function() {
  

  if (ctrl != nullptr) {
    if (checkAnsGetReceived(ctrl->getRxBuffer(), ANS_GET_CHANNEL_FUNCTION,
                            LEN_ANS_GET_CHANNEL_FUNCTION)) {

      iregs[ADD_OA_PIN_OUTPUT] = ctrl->getRx(ANS_GET_CHANNEL_FUNCTION_CH_POS);
      iregs[ADD_CH_FUNCTION] = ctrl->getRx(ANS_GET_CHANNEL_FUNCTION_FUN_POS);
      return true;
    }
  }
  return false;

}


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t AnalogExpansion::msg_set_led() {
  if( iregs[ADD_OA_PIN] >= OA_LED_NUM && 
    index >= OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    return 0;
  }
  if (ctrl != nullptr) {
    if (addressExist(ADD_OA_LED_VALUE)) {
      ctrl->setTx(iregs[ADD_OA_LED_VALUE], OA_SET_LED_VALUE_POS);
      uint8_t rv = prepareSetMsg(ctrl->getTxBuffer(), ARG_OA_SET_LED,
                                 LEN_OA_SET_LED);

      cfgs[index].backup(ctrl->getTxBuffer(), 
                         OFFSET_LED_VALUE, 
                         rv);
      return rv;
    }
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::analogWrite(uint8_t ch, uint32_t value,
                                  uint32_t period /*= 0*/,
                                  bool update /* = true*/) {
  if (ch < OA_AN_CHANNELS_NUM) {
    if (value > 8191) {
      value = 8191;
    }
    setDac(ch, value, update);
  } else if (ch >= OA_PWM_CH_0 && ch <= OA_PWM_CH_3) {
    setPwm(ch, value, period);
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

PinStatus AnalogExpansion::digitalRead(int pin, bool update /*= false*/) {
  if (pin >= 0 && pin < OA_AN_CHANNELS_NUM) {
    unsigned int v = 0;

    if (update) {
      execute(GET_DIGITAL_INPUT);
    }

    read(BASE_OA_DI_ADDRESS + pin, v);
    if (v == 1) {
      return HIGH;
    }
  }
  return LOW;
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::updateDigitalInputs() { execute(GET_DIGITAL_INPUT); }

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::updateAnalogInputs() {
  /*uint32_t err =*/execute(GET_ALL_ANALOG_INPUT);
  /*Serial.println("GET ALL ANALOG INPUT err = " + String(err));*/
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t AnalogExpansion::msg_get_all_ai() {
  return prepareGetMsg(ctrl->getTxBuffer(), ARG_OA_GET_ALL_ADC,
                       LEN_OA_GET_ALL_ADC);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool AnalogExpansion::parse_ans_get_all_ai() {
  if (checkAnsGetReceived(ctrl->getRxBuffer(), ANS_ARG_OA_GET_ALL_ADC,
                          ANS_LEN_OA_GET_ALL_ADC)) {
    const int s = ANS_OA_ADC_GET_ALL_VALUE_POS;
    for (int ch = 0; ch < OA_AN_CHANNELS_NUM; ch++) {
      iregs[BASE_OA_ADC_ADDRESS + ch] = ctrl->getRx(s + 2 * ch);
      iregs[BASE_OA_ADC_ADDRESS + ch] +=
          ((uint16_t)ctrl->getRx(s + 2 * ch + 1) << 8);
    }
    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t AnalogExpansion::msg_get_di() {
  return prepareGetMsg(ctrl->getTxBuffer(), ARG_OA_GET_DI, LEN_OA_GET_DI);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool AnalogExpansion::parse_ans_get_di() {
  if (checkAnsGetReceived(ctrl->getRxBuffer(), ANS_ARG_OA_GET_DI,
                          ANS_LEN_OA_GET_DI)) {
    iregs[ADD_OA_DI_VALUE] = ctrl->getRx(ANS_OA_GET_DI_VALUE_POS);
    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

int AnalogExpansion::analogRead(int pin, bool update /* = true*/) {
  if (pin < OA_AN_CHANNELS_NUM) {
    if (!addressExist(BASE_OA_ADC_ADDRESS + pin) || update) {
      return getAdc(pin, update);
    }
  }
  return iregs[BASE_OA_ADC_ADDRESS + pin];
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool AnalogExpansion::parse_oa_ack() {
  if (ctrl != nullptr) {

#ifdef DEBUG_PARSE_OA_ACK
    for (int i = 0; i < CTRL_ANS_OA_LEN; i++) {
      Serial.print(ctrl->getRxBuffer()[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
#endif

    return checkAnsSetReceived(ctrl->getRxBuffer(), ANS_ARG_OA_ACK,ANS_LEN_OA_ACK);
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool AnalogExpansion::verify_address(unsigned int add) {
  if (add == ADD_OA_PIN) {
    return true;
  } else if (add >= ADD_OA_ADC_TYPE && add <= ADD_OA_DAC_SLEW_RATE) {
    return true;
  } else if (add >= BASE_OA_PWM_ADDRESS && add <= ADD_OA_PWM_PULSE_3) {
    return true;
  } else if (add >= ADD_OA_ADC_VALUE_0 && add <= ADD_OA_ADC_VALUE_7) {
    return true;
  } else if (add >= ADD_OA_DAC_VALUE_0 && add <= ADD_OA_DAC_VALUE_7) {
    return true;
  } else if (add >= ADD_OA_RTD_VALUE_0 && add <= ADD_OA_RTD_VALUE_7) {
    return true;
  } else if (add >= ADD_OA_DI_VALUE_0 && add <= ADD_OA_DI_VALUE) {
    return true;
  } else if (add >= ADD_OA_LED_PIN_0 && add <= ADD_OA_LED_VALUE) {
    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

unsigned int AnalogExpansion::execute(uint32_t what) {
  i2c_rv = EXECUTE_OK;
  if (ctrl != nullptr) {
    switch (what) {

    case BEGIN_CHANNEL_AS_ADC:
      I2C_TRANSACTION(msg_begin_adc,
                      parse_oa_ack, getExpectedAnsLen(ANS_LEN_OA_ACK));
      break;
    case BEGIN_CHANNEL_AS_DI:
      I2C_TRANSACTION(msg_begin_di,
                      parse_oa_ack, getExpectedAnsLen(ANS_LEN_OA_ACK));
      break;
    case BEGIN_CHANNEL_AS_RTD:
      I2C_TRANSACTION(msg_begin_rtd,
                      parse_oa_ack, getExpectedAnsLen(ANS_LEN_OA_ACK));
      break;
    case BEGIN_CHANNEL_AS_DAC:
      I2C_TRANSACTION(msg_begin_dac,
                      parse_oa_ack, getExpectedAnsLen(ANS_LEN_OA_ACK));
      break;
    case SET_PWM:
      I2C_TRANSACTION(msg_set_pwm,
                      parse_oa_ack, getExpectedAnsLen(ANS_LEN_OA_ACK));
      break;
    case GET_SINGLE_ANALOG_INPUT:
      I2C_TRANSACTION(msg_get_adc,
                      parse_ans_get_adc,
                      getExpectedAnsLen(ANS_LEN_OA_GET_ADC));
      break;
    case SET_SINGLE_ANALOG_OUTPUT:
      I2C_TRANSACTION(msg_set_dac,
                      parse_oa_ack, getExpectedAnsLen(ANS_LEN_OA_ACK));
      break;
    case SEND_TIMING:
      I2C_TRANSACTION(msg_send_time,
                      parse_oa_ack, getExpectedAnsLen(ANS_LEN_OA_ACK));
      break;
    case GET_RTD:
      I2C_TRANSACTION(msg_get_rtd,
                      parse_ans_get_rtd,
                      getExpectedAnsLen(ANS_LEN_OA_GET_RTD));
      break;
    case SET_LED:
      I2C_TRANSACTION(msg_set_led,
                      parse_oa_ack, getExpectedAnsLen(ANS_LEN_OA_ACK));
      break;
    case GET_DIGITAL_INPUT:
      I2C_TRANSACTION(msg_get_di,
                      parse_ans_get_di,
                      getExpectedAnsLen(ANS_LEN_OA_GET_DI));
      break;
    case GET_ALL_ANALOG_INPUT:
      I2C_TRANSACTION(msg_get_all_ai,
                      parse_ans_get_all_ai,
                      getExpectedAnsLen(ANS_LEN_OA_GET_ALL_ADC));
      break;
    case SET_ALL_ANALOG_OUTPUTS:
      I2C_TRANSACTION(msg_set_all_dac,
                      parse_oa_ack, getExpectedAnsLen(ANS_LEN_OA_ACK));
      break;
    case BEGIN_CHANNEL_AS_HIGH_IMP:
      I2C_TRANSACTION(msg_begin_high_imp,
                      parse_oa_ack, getExpectedAnsLen(ANS_LEN_OA_ACK));
      break;
    case GET_CHANNEL_FUNCTION:
      I2C_TRANSACTION(msg_get_ch_function,
                      parse_get_ch_function, 
                      getExpectedAnsLen(LEN_ANS_GET_CHANNEL_FUNCTION));
      break;  
    
    default:
      i2c_rv = Expansion::execute(what);
      break;
    }
    ctrl->updateRegs(*this);
  } else {
    i2c_rv = EXECUTE_ERR_NO_CONTROLLER;
  }
  return i2c_rv;
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::write(unsigned int address, unsigned int value) {
  if (!verify_address(address)) {
    return;
  }

  if (address >= BASE_OA_ADC_ADDRESS && address <= ADD_OA_ADC_VALUE_7) {
    return; // READ ONLY ADDRESS
  }

  if (address >= BASE_OA_RTD_ADDRESS && address <= ADD_OA_RTD_VALUE_7) {
    return; // READ ONLY ADDRESS
  }
  if (address >= BASE_OA_DI_ADDRESS && address <= ADD_OA_DI_VALUE) {
    return; // READ ONLY ADDRESS
  }
  if (address >= ADD_OA_LED_PIN_0 && address <= ADD_OA_LED_PIN_7) {
    int pin = address - BASE_OA_LED_ADDRESS;
    if (pin < OA_LED_NUM) {
      if (value == 0) {
        iregs[ADD_OA_LED_VALUE] &= ~(1 << pin);
      } else {
        iregs[ADD_OA_LED_VALUE] |= (1 << pin);
      }
    }
  }
  return Expansion::write(address, value);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool AnalogExpansion::read(unsigned int address, unsigned int &value) {
  if (!verify_address(address)) {
    return false;
  }

  if (address >= ADD_OA_DI_VALUE_0 && address <= ADD_OA_DI_VALUE_7) {
    int pin = address - BASE_OA_DI_ADDRESS;
    if (iregs[ADD_OA_DI_VALUE] & (1 << pin)) {
      value = 1;
    } else {
      value = 0;
    }
    return true;
  }

  return Expansion::read(address, value);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void AnalogExpansion::beginChannelAsAdc(Controller &ctrl, uint8_t device,
                                        uint8_t ch, OaAdcType_t type,
                                        bool pull_down, bool rejection,
                                        bool diagnostic, uint8_t ma) {
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    AnalogExpansion ae = ctrl.getExpansion(device);
    if(ae) {
      ae.beginChannelAsAdc(ch, type, pull_down, rejection, diagnostic, ma);
    }
    else {
      AnalogExpansion *exp = (AnalogExpansion *)AnalogExpansion::makeExpansion();
      if(exp != nullptr) {
        exp->setIndex(device);
        exp->setCtrl(&ctrl);
        exp->beginChannelAsAdc(ch, type, pull_down, rejection, diagnostic, ma);
        delete exp;
      }
    }
  }
}

void AnalogExpansion::addAdcOnChannel(Controller &ctrl, uint8_t device,
                                      uint8_t ch, OaAdcType_t type,
                                      bool pull_down, bool rejection,
                                      bool diagnostic, uint8_t ma) {
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    AnalogExpansion ae = ctrl.getExpansion(device);
    if(ae) {
      ae.addAdcOnChannel(ch, type, pull_down, rejection, diagnostic, ma);
    }
    else {
      AnalogExpansion *exp = (AnalogExpansion *)AnalogExpansion::makeExpansion();
      if(exp != nullptr) {
        exp->setIndex(device);
        exp->setCtrl(&ctrl);
        exp->addAdcOnChannel(ch, type, pull_down, rejection, diagnostic, ma);
        delete exp;
      }
    }
  }
}
void AnalogExpansion::addVoltageAdcOnChannel(Controller &ctrl, uint8_t device,
                                             uint8_t ch) {
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    AnalogExpansion ae = ctrl.getExpansion(device);
    if(ae) {
      ae.addVoltageAdcOnChannel(ch);
    }
    else {
      AnalogExpansion *exp = (AnalogExpansion *)AnalogExpansion::makeExpansion();
      if(exp != nullptr) {
        exp->setIndex(device);
        exp->setCtrl(&ctrl);
        exp->addVoltageAdcOnChannel(ch);
        delete exp;
      }
    }
  }
}

void AnalogExpansion::addCurrentAdcOnChannel(Controller &ctrl, uint8_t device,
                                             uint8_t ch) {
  (void)ctrl;
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    AnalogExpansion ae = ctrl.getExpansion(device);
    if(ae) {
      ae.addCurrentAdcOnChannel(ch);
    }
    else {
      AnalogExpansion *exp = (AnalogExpansion *)AnalogExpansion::makeExpansion();
      if(exp != nullptr) {
        exp->setIndex(device);
        exp->setCtrl(&ctrl);
        exp->addCurrentAdcOnChannel(ch);
        delete exp;
      }
    }
  }
}
void AnalogExpansion::beginChannelAsVoltageAdc(Controller &ctrl, uint8_t device,
                                               uint8_t ch) {
  AnalogExpansion::beginChannelAsAdc(ctrl, device, ch, OA_VOLTAGE_ADC, true,
                                     true, false, 0);
}

void AnalogExpansion::beginChannelAsCurrentAdc(Controller &ctrl, uint8_t device,
                                               uint8_t ch) {
  AnalogExpansion::beginChannelAsAdc(ctrl, device, ch, OA_CURRENT_ADC, false,
                                     true, false, 0);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::beginChannelAsDigitalInput(
    Controller &ctrl, uint8_t device, uint8_t ch, bool filter, bool invert,
    bool simple_deb, OaDiSinkCurrent_t sink_cur, OaDebounceTime_t deb_time,
    bool scale, float th, float Vcc) {
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    AnalogExpansion ae = ctrl.getExpansion(device);
    if(ae) {
      ae.beginChannelAsDigitalInput(ch, filter, invert, simple_deb, sink_cur,
                                   deb_time, scale, th, Vcc);
    }
    else {
      AnalogExpansion *exp = (AnalogExpansion *)AnalogExpansion::makeExpansion();
      if(exp != nullptr) {
        exp->setIndex(device);
        exp->setCtrl(&ctrl);
        exp->beginChannelAsDigitalInput(ch, filter, invert, simple_deb, sink_cur,
                                     deb_time, scale, th, Vcc);
        delete exp;
      }
    }
  }
}
void AnalogExpansion::beginChannelAsDigitalInput(Controller &ctrl,
                                                 uint8_t device, uint8_t ch) {
  AnalogExpansion::beginChannelAsDigitalInput(
      ctrl, device, ch, true, false, true, OA_DI_SINK_1, OA_DI_DEB_TIME_24,
      false, 5.2, 16.0);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::beginChannelAsRtd(Controller &ctrl, uint8_t device,
                                        uint8_t ch, bool use_3_wires,
                                        float current) {
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    AnalogExpansion ae = ctrl.getExpansion(device);
    if(ae) {
      ae.beginChannelAsRtd(ch, use_3_wires, current);
    }
    else {
    AnalogExpansion *exp = (AnalogExpansion *)AnalogExpansion::makeExpansion();
      if(exp != nullptr) {
        exp->setIndex(device);
        exp->setCtrl(&ctrl);
        exp->beginChannelAsRtd(ch, use_3_wires, current);
        delete exp;
      }
    }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::beginRtdUpdateTime(Controller &ctrl, uint8_t device,
                                         uint16_t time) {
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    AnalogExpansion ae = ctrl.getExpansion(device);
    if(ae) {
      ae.beginRtdUpdateTime(time);
    }
    else {
      AnalogExpansion *exp = (AnalogExpansion *)AnalogExpansion::makeExpansion();
      if(exp != nullptr) {
        exp->setIndex(device);
        exp->setCtrl(&ctrl);
        exp->beginRtdUpdateTime(time);
        delete exp;
      }
    }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::beginChannelAsDac(Controller &ctrl, uint8_t device,
                                        uint8_t ch, OaDacType_t type,
                                        bool limit_current, bool enable_slew,
                                        OaDacSlewRate_t sr) {
 
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    AnalogExpansion ae = ctrl.getExpansion(device);
    if(ae) {
      ae.beginChannelAsDac(ch, type, limit_current, enable_slew, sr);
    }
    else {
      AnalogExpansion *exp = (AnalogExpansion *)AnalogExpansion::makeExpansion();
      if(exp != nullptr) {
        exp->setIndex(device);
        exp->setCtrl(&ctrl);
        exp->beginChannelAsDac(ch, type, limit_current, enable_slew, sr);
        delete exp;
      }
    }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::beginChannelAsVoltageDac(Controller &ctrl, uint8_t device,
                                               uint8_t ch) {
  AnalogExpansion::beginChannelAsDac(ctrl, device, ch, OA_VOLTAGE_DAC, true,
                                     false, OA_SLEW_RATE_0);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::beginChannelAsCurrentDac(Controller &ctrl, uint8_t device,
                                               uint8_t ch) {
  AnalogExpansion::beginChannelAsDac(ctrl, device, ch, OA_CURRENT_DAC, false,
                                     false, OA_SLEW_RATE_0);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::setTimeoutForDefaultValues(Controller &ctrl, uint8_t device, uint16_t timeout_ms) {
  
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    AnalogExpansion ae = ctrl.getExpansion(device);
    if(ae) {
      ae.setTimeoutForDefaultValues(timeout_ms);
    }
    else {
      AnalogExpansion *exp = (AnalogExpansion *)AnalogExpansion::makeExpansion();
      if(exp != nullptr) {
        /* create a temporary object just to set configuration */
        exp->setIndex(device);
        exp->setCtrl(&ctrl);
        exp->setTimeoutForDefaultValues(timeout_ms);
        delete exp;
      }
    }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::setDefaultDac(Controller &ctrl, uint8_t device, uint8_t ch, uint16_t value) {
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    AnalogExpansion ae = ctrl.getExpansion(device);
    if(ae) {
      ae.setDefaultDac(ch, value);
    }
    else {
      AnalogExpansion *exp = (AnalogExpansion *)AnalogExpansion::makeExpansion();
      if(exp != nullptr) {
        exp->setIndex(device);
        exp->setCtrl(&ctrl);
        exp->setDefaultDac(ch, value);
        delete exp;
      }
    }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::setDefaultPwm(Controller &ctrl, uint8_t device, uint8_t ch, uint32_t period, uint32_t pulse) {
  
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM ) {
    AnalogExpansion ae = ctrl.getExpansion(device);
    if(ae) {
      ae.setDefaultPwm(ch, period, pulse);
    }
    else {
      AnalogExpansion *exp = (AnalogExpansion *)AnalogExpansion::makeExpansion();
      if(exp != nullptr) {
        exp->setIndex(device);
        exp->setCtrl(&ctrl);
        exp->setDefaultPwm(ch, period, pulse);
        delete exp;
      }
    }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void AnalogExpansion::setProductData(uint8_t *data, uint8_t len) {
  Expansion::set_flash_data(data, len, PRODUCTION_DATA_FLASH_ADDRESS);
}

bool AnalogExpansion::isChDac(uint8_t ch, bool actual_hw) {
  if (index < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    if(actual_hw) {
      CfgFun_t fun = get_channel_function(ch);
      return ((fun == CH_FUNC_VOLTAGE_OUTPUT) || (fun == CH_FUNC_CURRENT_OUTPUT)); 
    }
    return (AnalogExpansion::cfgs[index].isVoltageDacCh(ch) ||
            AnalogExpansion::cfgs[index].isCurrentDacCh(ch));
  }
  return false;
}

bool AnalogExpansion::isChAdc(uint8_t ch, bool actual_hw) {
  if (index < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    if(actual_hw) {
      CfgFun_t fun = get_channel_function(ch);
      return ((fun == CH_FUNC_VOLTAGE_INPUT) || (fun == CH_FUNC_CURRENT_INPUT_EXT_POWER)); 
    }
    return (AnalogExpansion::cfgs[index].isVoltageAdcCh(ch) ||
            AnalogExpansion::cfgs[index].isCurrentAdcCh(ch));
  }
  return false;
}

bool AnalogExpansion::isChVoltageDac(uint8_t ch, bool actual_hw) {
  if (index < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    if(actual_hw) {
      CfgFun_t fun = get_channel_function(ch);
      return ((fun == CH_FUNC_VOLTAGE_OUTPUT)); 
    }
    return (AnalogExpansion::cfgs[index].isVoltageDacCh(ch));
  }
  return false;
}

bool AnalogExpansion::isChCurrentDac(uint8_t ch, bool actual_hw) {
  if (index < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    if(actual_hw) {
      CfgFun_t fun = get_channel_function(ch);
      return ((fun == CH_FUNC_CURRENT_OUTPUT)); 
    }
    return (AnalogExpansion::cfgs[index].isCurrentDacCh(ch));
  }
  return false;
}

bool AnalogExpansion::isChVoltageAdc(uint8_t ch, bool actual_hw) {
  if (index < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    if(actual_hw) {
      CfgFun_t fun = get_channel_function(ch);
      return (fun == CH_FUNC_VOLTAGE_INPUT); 
    }
    return (AnalogExpansion::cfgs[index].isVoltageAdcCh(ch));
  }
  return false;
}

bool AnalogExpansion::isChCurrentAdc(uint8_t ch, bool actual_hw) {
  if (index < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    if(actual_hw) {
      CfgFun_t fun = get_channel_function(ch);
      return (fun == CH_FUNC_CURRENT_INPUT_EXT_POWER); 
    }
    return (AnalogExpansion::cfgs[index].isCurrentAdcCh(ch));
  }
  return false;
}

bool AnalogExpansion::isChDigitalInput(uint8_t ch, bool actual_hw) {
  if (index < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    if(actual_hw) {
      CfgFun_t fun = get_channel_function(ch);
      return (fun == CH_FUNC_DIGITAL_INPUT); 
    }
    return (AnalogExpansion::cfgs[index].isDigitalInputCh(ch));
  }
  return false;
}

bool AnalogExpansion::isChRtd(uint8_t ch, bool actual_hw) {
  if (index < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    if(actual_hw) {
      CfgFun_t fun = get_channel_function(ch);
      return ((fun == CH_FUNC_RESISTANCE_MEASUREMENT) || (fun == CH_FUNC_RESISTANCE_MEASUREMENT_3_WIRES)); 
    }
    return (AnalogExpansion::cfgs[index].isRtdCh(ch));
  }
  return false;
}

bool AnalogExpansion::isChRtd2Wires(uint8_t ch, bool actual_hw) {
  if (index < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    if(ch > 1) {
      return (AnalogExpansion::cfgs[index].isRtdCh(ch));
    }
    else {
      if(actual_hw) {
        CfgFun_t fun = get_channel_function(ch);
        return (fun == CH_FUNC_RESISTANCE_MEASUREMENT); 
      }
      if(AnalogExpansion::cfgs[index].isRtdCh(ch) && !AnalogExpansion::cfgs[index].isRtd3WiresCh(ch)) {
        return true;
      }

    }
  }
  return false;

}


bool AnalogExpansion::isChRtd3Wires(uint8_t ch, bool actual_hw) {
  if (index < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    if(actual_hw) {
      CfgFun_t fun = get_channel_function(ch);
      return (fun == CH_FUNC_RESISTANCE_MEASUREMENT_3_WIRES); 
    }
    return (AnalogExpansion::cfgs[index].isRtd3WiresCh(ch));
  }
  return false;

}

bool AnalogExpansion::isChHighImpedance(uint8_t ch, bool actual_hw) {

  if (index < OPTA_CONTROLLER_MAX_EXPANSION_NUM && ch < OA_AN_CHANNELS_NUM) {
    if(actual_hw) {
      CfgFun_t fun = get_channel_function(ch);
      return (fun == CH_FUNC_HIGH_IMPEDENCE); 
    }
    return (AnalogExpansion::cfgs[index].isHighImpedanceCh(ch));
  }
  return false;
}

} // namespace Opta
#endif
