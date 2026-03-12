/* -------------------------------------------------------------------------- */
/* FILENAME:    OptaAnalog.cpp
   AUTHOR:      Daniele Aimo (d.aimo@arduino.cc)
   DATE:        20230801
   REVISION:    0.0.1
   DESCRIPTION: Implementation of the class OptaAnalog
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "AnalogCommonCfg.h"
#include <cstdint>
#include <stdint.h>

#define GET_DEVICE_FROM_CHANNEL(ch)   ((ch >= 2 && ch <= 5) ? OA_AN_DEVICE_1 : OA_AN_DEVICE_0)

#ifndef ARDUINO_OPTA

#if defined ARDUINO_OPTA_ANALOG || defined ARDUINO_UNO_TESTALOG_SHIELD

#include "OptaAnalog.h"

#define FRAME_SIZE 4

#ifdef DEBUG_SERIAL
void dbgPrintHex(uint8_t h, bool space = false, bool nl = false) {
  Serial.print("0x");
  if (h < 0x10) {
    Serial.print("0");
  }
  Serial.print(h, HEX);
  if (space)
    Serial.print(" ");
  if (nl)
    Serial.println();
}

void dbgPrintHex(uint16_t h, bool space = false, bool nl = false) {
  Serial.print("0x");
  if (h < 0x10) {
    Serial.print("000");
  } else if (h < 0x100) {
    Serial.print("00");
  } else if (h < 0x1000) {
    Serial.print("0");
  }

  Serial.print(h, HEX);

  if (space)
    Serial.print(" ");
  if (nl)
    Serial.println();
}

void printSingleReg(OptaAnalog *oa, uint8_t device, uint8_t addr) {
  uint16_t value = 0;
  value = oa->readReg(device, addr);
  Serial.print("[");
  Serial.print(device);
  Serial.print("][");
  dbgPrintHex(addr, true);
  Serial.print("] = ");
  dbgPrintHex(value, true, true);
}

/* -------------------------------------------------------------------------- */
void printFourReg(OptaAnalog *oa, uint8_t device, uint8_t r, String desc) {
  Serial.println(desc);
  for (int i = 0; i < 4; i++) {
    printSingleReg(oa, device, r + (uint8_t)i);
  }
  Serial.println();
}
#endif
/*                        FUNCTIONS RELATED TO PWM                            */
/* -------------------------------------------------------------------------- */
/* PWM0_A - ch1 of first PWM IC    P107   [D4]   32 bit
 * PWM3_A - ch2 of first PWM IC    P111   [D7]   16 bit
 * IRQ3 - Fault of first PWM IC    P110   [D8]
 *
 * PWM2_A - ch1 of second PWM IC   P500   [D6]   16 bit
 * PWM1_A - ch2 of second PWM IC   P105   [D5]   32 bit
 * IRQ11 - Fault of second PWM IC  P501   [D9]
 *
 * PWM clock is 48 MHz
 * The maximum divider is 64
 * So the minimum frequency is 48 MHz / 64 = 750 kHz
 * The maximum period is 2^32 / 750 kHz = 5726 s for 32 bit PWM
 * The maximum period is 2^16 / 750 kHz = 87 ms for 16 bit PWM
 * */

/* -------------------------------------------------------------------------- */
void OptaAnalog::begin_pwms() {
  /* ------------------------------------------------------------------------ */
  for (int i = 0; i < OA_PWM_CHANNELS_NUM; i++) {
    pwm[i].begin();
  }
}

/* -------------------------------------------------------------------------- */
void OptaAnalog::configurePwmPeriod(uint8_t ch, uint32_t _period_us) {
  /* ------------------------------------------------------------------------ */
  if (ch >= OA_PWM_CHANNELS_NUM) {
    return;
  }

  pwm[ch].set_period_us = _period_us;
}

/* -------------------------------------------------------------------------- */
void OptaAnalog::configurePwmPulse(uint8_t ch, uint32_t _pulse_us) {
  /* ------------------------------------------------------------------------ */

  if (ch >= OA_PWM_CHANNELS_NUM) {
    return;
  }
  pwm[ch].set_pulse_us = _pulse_us;
}

/* -------------------------------------------------------------------------- */
void OptaAnalog::updatePwm(uint8_t ch) {
/* ------------------------------------------------------------------------ */
  
  if (ch >= OA_PWM_CHANNELS_NUM) {
    return;
  }
  
  if (pwm[ch].set_period_us == 0) {
    pwm[ch].pwm.suspend();
  } 
  else {
    if (pwm[ch].set_pulse_us <= pwm[ch].set_period_us) {
      if (pwm[ch].set_period_us != pwm[ch].period_us || pwm[ch].set_pulse_us != pwm[ch].pulse_us) {
        pwm[ch].pwm.resume();
        pwm[ch].pwm.period_us(pwm[ch].set_period_us);
        pwm[ch].pwm.pulseWidth_us(pwm[ch].set_pulse_us);      
      }
    }
  }
  pwm[ch].period_us = pwm[ch].set_period_us;
  pwm[ch].pulse_us = pwm[ch].set_pulse_us;
}

/* -------------------------------------------------------------------------- */
void OptaAnalog::updatePwmWithDefault(uint8_t ch) {
/* ------------------------------------------------------------------------ */
  
  if (ch >= OA_PWM_CHANNELS_NUM) {
    return;
  }

  /* only if PWM is used */
  
  if(pwm_period_defaults[ch] == 0) {
    pwm[ch].pwm.suspend();
  } 
  else {
    if (pwm_pulse_defaults[ch] <= pwm_period_defaults[ch]) {
      if (pwm_period_defaults[ch] != pwm[ch].period_us || pwm_pulse_defaults[ch] != pwm[ch].pulse_us) {
        pwm[ch].pwm.resume();
        pwm[ch].pwm.period_us(pwm_period_defaults[ch]);
        pwm[ch].pwm.pulseWidth_us(pwm_pulse_defaults[ch]);      
      }
    }
  }
  
  pwm[ch].period_us = pwm_period_defaults[ch];
  pwm[ch].pulse_us = pwm_pulse_defaults[ch];
}


/* -------------------------------------------------------------------------- */
void OptaAnalog::suspendPwm(uint8_t ch) {
  /* ------------------------------------------------------------------------ */
  if (ch >= OA_PWM_CHANNELS_NUM) {
    return;
  }

  pwm[ch].pwm.suspend();
  pwm[ch].period_us = 0;
  pwm[ch].pulse_us = 0;
  pwm[ch].set_period_us = 0;
  pwm[ch].set_pulse_us = 0;

}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void OptaAnalog::setLedStatus(uint8_t i) {
  PinStatus hw_st = (led_status & (1 << i)) ? HIGH : LOW;
  /* this take advantage from the definition of LED position in the Opta
   * Analog variant */
  digitalWrite(OA_VARIANT_FIST_LED_POS + i, hw_st);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void OptaAnalog::updateLedStatus() {
  for (int i = 0; i < OA_NUMBER_OF_LEDS; i++) {
    setLedStatus(i);
  }
}

/*                            CONSTRUCTOR */

/* Note: PWM_x are defined in the variant of OPTA Analog, they are defined
 * in an order so that PWM_0 correspond to PWM ch 0 which is the leftmost on
 * the Opta analog connector, PWM_1 is the second Pin and so on (but take
 * into account that third and fourth pins are ground) To obtain the correct
 * correspondent is necessary that PWM_x are used "in order" from 0 to 3 in
 * the Constructor here below */

/* --------------------------------------------------------------------------
 */
OptaAnalog::OptaAnalog()
    : pwm{CfgPwm(PWM_0), CfgPwm(PWM_1), CfgPwm(PWM_2), CfgPwm(PWM_3)},
      led_status(0), rtd_update_time(1000) {
  expansion_type = EXPANSION_OPTA_ANALOG;
  for (int i = 0; i < OA_AN_CHANNELS_NUM; i++) {
    fun[i] = CH_FUNC_HIGH_IMPEDENCE;
    dac_value_updated[i] = true;
    dac_values[i] = 0;
    dac_defaults[i] = 0;
  }

  for (int i = 0; i < OA_PWM_CHANNELS_NUM; i++) {
    pwm_period_defaults[i] = 0;
    pwm_pulse_defaults[i] = 0;

  }
  
  /* ------------------------------------------------------------------------ */
}


/* BEGIN */
/* -------------------------------------------------------------------------- */
void OptaAnalog::begin() {
  /* ------------------------------------------------------------------------ */

  digitalWrite(DIO_RESET_1,LOW);
  digitalWrite(DIO_RESET_2,LOW);
  delay(10);
  digitalWrite(DIO_RESET_1,HIGH);
  digitalWrite(DIO_RESET_2,HIGH);

  set_up_timer();

  Module::begin();
  SPI.begin();

#ifdef ARDUINO_OPTA_ANALOG
  /* chip select set up in variant */

  /* pwm initialization */
  begin_pwms();

  /* GPO initialization  */
  configureGpo(0, GPO_HIGH_IMP, GPO_LOW);
  updateGpo(0);
  configureGpo(1, GPO_HIGH_IMP, GPO_LOW);
  updateGpo(1);
  configureGpo(2, GPO_HIGH_IMP, GPO_LOW);
  updateGpo(2);
  configureGpo(3, GPO_HIGH_IMP, GPO_LOW);
  updateGpo(3);
  configureGpo(4, GPO_HIGH_IMP, GPO_LOW);
  updateGpo(4);
  configureGpo(5, GPO_HIGH_IMP, GPO_LOW);
  updateGpo(5);
  configureGpo(6, GPO_HIGH_IMP, GPO_LOW);
  updateGpo(6);
  configureGpo(7, GPO_HIGH_IMP, GPO_LOW);
  updateGpo(7);

#else
#ifdef DEBUG_SERIAL
  Serial.println("*** OPTA ANALOG TEST VERSION, runs on UNO R4 MINIMA ***");
  Serial.println("-------------------------------------------------------");
#endif
  /* chip select */
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  delay(PRE_AN_DEV_RESET_TIME);
  pinMode(DIO_RESET, OUTPUT);
  digitalWrite(DIO_RESET, LOW);
  delay(AN_DEV_RESET_TIME);
  digitalWrite(DIO_RESET, HIGH);
  delay(POST_AN_DEV_RESET_TIME);

  configureGpo(0, GPO_HIGH_IMP, GPO_LOW);
  updateGpo(0);
  configureGpo(1, GPO_HIGH_IMP, GPO_LOW);
  updateGpo(1);
  configureGpo(2, GPO_HIGH_IMP, GPO_LOW);
  updateGpo(2);
  configureGpo(3, GPO_HIGH_IMP, GPO_LOW);
  updateGpo(3);

#endif
}

#ifdef DEBUG_UPDATE_FW
/* used to recognize sw version with led blinking during fw update tests*/
void OptaAnalog::debug_with_leds() {
  static unsigned long start = millis();
  static bool status = true;

  if (millis() - start > 2000) {
    start = millis();

    if (status) {
      set_led_on(0);
      status = false;
    } else {
      set_led_off(0);
      status = true;
    }
  }
}
#endif

void OptaAnalog::update() {
  Module::update();

  /* Opta analog has a slow main that depends on the operation it is performing
     at the very beginning (when the espansion has not an address yet) it is 
     necessary to go faster so the main is skipped untill the expansion gets 
     a valid address */

  if(wire_i2c_address <= OPTA_DEFAULT_SLAVE_I2C_ADDRESS || wire_i2c_address >= OPTA_CONTROLLER_FIRST_TEMPORARY_ADDRESS) {
    return;
  }

#ifdef DEBUG_UPDATE_FW
  debug_with_leds();
  updateLedStatus();
#endif
  setup_channels();
  Module::update();
  // unsigned long time = millis();

  // MEMO: always first update DAC and then rtd
  // This ensure synchronization
  if(use_default_output_values) {
    for(int i = 0; i < OA_AN_CHANNELS_NUM; i++) {
      if(is_dac_used(i)) {
        write_dac_defaults(i); 
      }
    }
    /* when use_default_output_values becomes false (because a message has been
       received) this ensure the DAC outputs are re-updated to the last values */
    update_dac_using_LDAC = true;
    toggle_ldac();
  }
  else {
    for (int i = 0; i < OA_AN_CHANNELS_NUM; i++) {
      updateDac(i);
      updateDacValue(i, false);
    }
    Module::update();
    if (update_dac_using_LDAC && are_all_dac_updated()) {
      update_dac_using_LDAC = false;
      
      toggle_ldac();
    }
  }
  Module::update();

  static unsigned long last_rtd_update = millis();
  if (millis() - last_rtd_update > rtd_update_time) {
    last_rtd_update = millis();
    updateRtd();
  }
  Module::update();

  if(use_default_output_values) {
    for(int i = 0; i < OA_PWM_CHANNELS_NUM; i++) {
      updatePwmWithDefault(i);
    }
  }
  else {
    for (int i = 0; i < OA_PWM_CHANNELS_NUM; i++) {
      updatePwm(i);
    }
  }
  Module::update();

  /* handle start and stop of adc on device 0*/
  if (adc_ch_mask_0_last != adc_ch_mask_0) {
    adc_ch_mask_0_last = adc_ch_mask_0;
    stop_adc(0, false);
    start_adc(0, false);
  }
  Module::update();

  /* handle start and stop of adc on device 1 */
  if (adc_ch_mask_1_last != adc_ch_mask_1) {
    adc_ch_mask_1_last = adc_ch_mask_1;
    stop_adc(1, false);
    start_adc(1, false);
  }
  Module::update();

  updateDinReadings();
  updateAdc(false);
  updateAdcDiagnostics();
  updateLedStatus();
  update_live_status();

  Module::update();
  // Serial.println("Update time: " + String(millis() - time));
}

/* ---------------------------------------------------------------------
 * SPI communication functions
 * --------------------------------------------------------------------- */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* set the offset for channels register */
uint8_t OptaAnalog::get_add_offset(uint8_t ch) {
  uint8_t rv = 0;
  if (ch < OA_AN_CHANNELS_NUM) {
#ifdef ARDUINO_UNO_TESTALOG_SHIELD
    rv = ch;
#else
    if (ch == 0) {
      rv = 1;
    } else if (ch == 1) {
      rv = 0;
    } else if (ch == 2) {
      rv = 0;
    } else if (ch == 3) {
      rv = 1;
    } else if (ch == 4) {
      rv = 2;
    } else if (ch == 5) {
      rv = 3;
    } else if (ch == 6) {
      rv = 2;
    } else if (ch == 7) {
      rv = 3;
    }
#endif
  }
  return rv;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::write_reg(uint8_t addr, uint16_t value, uint8_t ch) {
  uint8_t ad = addr + get_add_offset(ch);
  uint8_t device = GET_DEVICE_FROM_CHANNEL(ch);
  write_direct_reg(device, ad, value);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool OptaAnalog::read_reg(uint8_t addr, uint16_t &value, uint8_t ch) {
  /* Bit 8 of read register: Determines the content of the MSBs in the
   * SPI read frame. When this bit is set to 0, the READBACK_ADDR is
   * returned in bits, Bits[30:24] (the MSB is not shown) of any
   * subsequent SPI read. When this bit is set to 1, the ADC_RDY bit,
   * alert flags, and the four digital input outputs are returned
   * in Bits[30:24] of any subsequent SPI read. */
  uint8_t ad = addr + get_add_offset(ch);
  uint8_t device = GET_DEVICE_FROM_CHANNEL(ch);
  return read_direct_reg(device, ad, value);
}

void OptaAnalog::write_direct_reg(uint8_t device, uint8_t addr,
                                  uint16_t value) {

  com_buffer[0] = addr;
  com_buffer[1] = value >> 8;
  com_buffer[2] = value & 0xFF;
  com_buffer[3] = OptaCrc8::calc(com_buffer, 3, 0);

#if defined DEBUG_SERIAL && defined DEBUG_SPI_COMM
  Serial.print("TX: ");
  for (int i = 0; i < 4; i++) {
    if (com_buffer[i] < 0x10) {
      Serial.print('0');
    }
    Serial.print(com_buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
#endif

#ifdef ARDUINO_UNO_TESTALOG_SHIELD
  digitalWrite(CS, LOW);
#else
  if (device == 0) {
    digitalWrite(SPI_CS_1, LOW);
  } else if (device == 1) {
    digitalWrite(SPI_CS_2, LOW);
  }
  delay(1);
#endif
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE1));
  SPI.transfer(com_buffer, SPI_COMM_BUFF_DIM);
  SPI.endTransaction();

#if defined DEBUG_SERIAL && defined DEBUG_SPI_COMM
  Serial.print("RX: ");
  for (int i = 0; i < SPI_COMM_BUFF_DIM; i++) {
    if (com_buffer[i] < 0x10) {
      Serial.print('0');
    }
    Serial.print(com_buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
#endif

#ifdef ARDUINO_UNO_TESTALOG_SHIELD
  digitalWrite(CS, HIGH);

#else
  if (device == 0) {
    digitalWrite(SPI_CS_1, HIGH);
  } else if (device == 1) {
    digitalWrite(SPI_CS_2, HIGH);
  }
#endif
}

bool OptaAnalog::read_direct_reg(uint8_t device, uint8_t addr,
                                 uint16_t &value) {
  write_direct_reg(device, OA_REG_READ_SELECT___SINGLE_PER_DEVICE, (addr | 0x100));
  write_direct_reg(device, OPTA_AN_NOP___SINGLE_PER_DEVICE, 0x00);
  if (OptaCrc8::verify(com_buffer[3], com_buffer, 3)) {
    value = com_buffer[2] | ((uint16_t)com_buffer[1] << 8);
    return true;
  }
  return false;
}
/* ################################################################### */
/* CONFIGURE CHANNEL FUNCTIONs                                         */
/* ################################################################### */

// this function "reset" channels in case they are used for DAC or
// resistance measurement
void OptaAnalog::configureFunction(uint8_t ch, CfgFun_t f, bool write) {
  if (ch < OA_AN_CHANNELS_NUM) {
    ChConfig cfg;
    cfg.write = write;
    cfg.f = f;
    update_fun[ch].push(cfg);
  }
}


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

CfgFun_t OptaAnalog::getFunction(uint8_t ch) {
  if (ch < OA_AN_CHANNELS_NUM) {
    return fun[ch];
  }
  return CH_FUNC_UNDEFINED;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void OptaAnalog::sendFunction(uint8_t ch, CfgFun_t f) {
  if (ch < OA_AN_CHANNELS_NUM) {
    uint8_t v = 0;

    switch (f) {
    case CH_FUNC_HIGH_IMPEDENCE:
      v = CH_HIGH_IMP;
      break;
    case CH_FUNC_VOLTAGE_OUTPUT:
      v = CH_VO;
      break;
    case CH_FUNC_CURRENT_OUTPUT:
      v = CH_CO;
      break;
    case CH_FUNC_VOLTAGE_INPUT:
      v = CH_VI;
      break;
    case CH_FUNC_CURRENT_INPUT_EXT_POWER:
      v = CH_CI_EP;
      break;
    case CH_FUNC_CURRENT_INPUT_LOOP_POWER:
      v = CH_CI_LP;
      break;
    case CH_FUNC_RESISTANCE_MEASUREMENT:
      v = CH_RM;
      break;
    case CH_FUNC_DIGITAL_INPUT:
      v = CH_DI;
      break;
    case CH_FUNC_DIGITAL_INPUT_LOOP_POWER:
      v = CH_DI_LP;
      break;
    default:
      v = CH_HIGH_IMP;
      break;
    }

    write_reg(OA_REG_FUNC_SETUP, v, ch);
    // Serial.println("Send funtion ch " + String(ch) + " value " +
    // String(v));
  }
}

/* ##########################################################################
 */
/*                            ADC FUNCTIONs */
/* ##########################################################################
 */

void OptaAnalog::configureAdcMux(uint8_t ch, CfgAdcMux_t m) {
  if (ch < OA_AN_CHANNELS_NUM) {
    adc[ch].mux = m;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureAdcRange(uint8_t ch, CfgAdcRange_t r) {
  if (ch < OA_AN_CHANNELS_NUM) {
    adc[ch].range = r;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureAdcPullDown(uint8_t ch, bool en) {
  if (ch < OA_AN_CHANNELS_NUM) {
    adc[ch].en_pull_down = en;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureAdcRejection(uint8_t ch, bool en) {
  if (ch < OA_AN_CHANNELS_NUM) {
    adc[ch].en_rejection = en;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureAdcDiagnostic(uint8_t ch, bool en) {
  if (ch < OA_AN_CHANNELS_NUM) {
    adc[ch].en_conversion_diagnostic = en;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureAdcDiagRejection(uint8_t ch, bool en) {
  uint8_t device = GET_DEVICE_FROM_CHANNEL(ch);
  en_adc_diag_rej[device] = en;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureAdcEnable(uint8_t ch, bool en) {
  if (ch < OA_AN_CHANNELS_NUM) {
    adc[ch].en_conversion = en;
    if (ch == 0 || ch == 1 || ch == 6 || ch == 7) {
      if (en) {
        adc_ch_mask_0 |= (1 << ch);
      } else {
        adc_ch_mask_0 &= ~(1 << ch);
      }
    } else if (ch == 2 || ch == 3 || ch == 4 || ch == 5) {
      if (en) {
        adc_ch_mask_1 |= (1 << ch);
      } else {
        adc_ch_mask_1 &= ~(1 << ch);
      }
    }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureAdcMovingAverage(uint8_t ch, uint8_t ma) {
  if (ch < OA_AN_CHANNELS_NUM) {
    adc[ch].mov_average_req = ma;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* Send the ADC configuration for CHANNEL ch to the Analog Device */
void OptaAnalog::sendAdcConfiguration(uint8_t ch) {

  uint16_t reg_cfg = 0;
  if (ch < OA_AN_CHANNELS_NUM) {

    if (adc[ch].mux == CFG_ADC_INPUT_NODE_IOP_AGND_SENSE) {

      ADC_CFG_SET_AD_SENSE(reg_cfg);
    } else if (adc[ch].mux == CFG_ADC_INPUT_NODE_100OHM_R) {

      ADC_CFG_SET_100_OHM(reg_cfg);
    }

    switch (adc[ch].range) {
    case CFG_ADC_RANGE_10V:

      ADC_CFG_SET_RANGE(reg_cfg, ADC_CFG_RANGE_10V);
      break;

    case CFG_ADC_RANGE_2_5V_RTD:

      ADC_CFG_SET_RANGE(reg_cfg, ADC_CFG_RANGE_2_5V_RTD);
      break;
    case CFG_ADC_RANGE_2_5V_LOOP:

      ADC_CFG_SET_RANGE(reg_cfg, ADC_CFG_RANGE_2_5V_LOOP);
      break;
    case CFG_ADC_RANGE_2_5V_BI:

      ADC_CFG_SET_RANGE(reg_cfg, ADC_CFG_RANGE_2_5V_BI);
      break;
    default:

      ADC_CFG_SET_RANGE(reg_cfg, ADC_CFG_RANGE_10V);
      break;
    }

    if (adc[ch].en_pull_down) {
      ADC_CFG_ENABLE_PULL_DOWN(reg_cfg);
    } else {
      ADC_CFG_DISABLE_PULL_DOWN(reg_cfg);
    }

    if (adc[ch].en_rejection) {
      ADC_CFG_ENABLE_REJECT(reg_cfg);
    } else {
      ADC_CFG_DISABLE_REJECT(reg_cfg);
    }

    write_reg(OA_REG_ADC_CONFIG, reg_cfg, ch);
  }
}

/* -------------------------------------------------------- */
bool OptaAnalog::is_adc_started(uint8_t device) {
  uint16_t r = 0;
  read_direct_reg(device, OA_REG_ADC_CONV_CTRL___SINGLE_PER_DEVICE, r);
  uint16_t v = (r & (3 << 8)) >> 8;
  if (v == 1 || v == 2) {
    return true;
  }
  return false;
}

/* -------------------------------------------------------- */
void OptaAnalog::stop_adc(uint8_t device, bool power_down) {
  /* stop adc 1 by writing stop in the ADC conversion control */
  uint16_t stop_value = 0;
  if (power_down) {
    stop_value = STOP_CONTINUOUS_MODE_AND_OFF;
  } else {
    stop_value = STOP_CONTINUOUS_MODE_AND_ON;
  }

  uint16_t r = 0;
  if (is_adc_started(device)) {
    do {
      write_direct_reg(device, OA_REG_ADC_CONV_CTRL___SINGLE_PER_DEVICE, stop_value);
      read_direct_reg(device, OA_REG_LIVE_STATUS___SINGLE_PER_DEVICE, r);
    } while (r & ADC_BUSY_MASK);
  }
}

bool OptaAnalog::adc_enable_channel(uint8_t ch, uint16_t &reg) {
  if (adc[ch].en_conversion) {
    ENABLE_ADC_CONVERSION(reg, get_add_offset(ch));
    if (adc[ch].en_conversion_diagnostic) {
      ENABLE_ADC_DIAG_CONVERSION(reg, ch);
    }
    return true;
  }
  return false;
}
/* -------------------------------------------------------- */
void OptaAnalog::start_adc(uint8_t device, bool single_acquisition) {
  uint16_t reg_cfg = 0;
  bool start = false;

  if (device == OA_AN_DEVICE_0) {
    start |= adc_enable_channel(OA_CH_0, reg_cfg);
    start |= adc_enable_channel(OA_CH_1, reg_cfg);
    start |= adc_enable_channel(OA_CH_6, reg_cfg);
    start |= adc_enable_channel(OA_CH_7, reg_cfg);
    if (en_adc_diag_rej[OA_AN_DEVICE_0]) {
      ENABLE_DIAG_REJECTION(reg_cfg);
    }
  } else if (device == OA_AN_DEVICE_1) {
    start |= adc_enable_channel(OA_CH_2, reg_cfg);
    start |= adc_enable_channel(OA_CH_3, reg_cfg);
    start |= adc_enable_channel(OA_CH_4, reg_cfg);
    start |= adc_enable_channel(OA_CH_5, reg_cfg);
    if (en_adc_diag_rej[OA_AN_DEVICE_1]) {
      ENABLE_DIAG_REJECTION(reg_cfg);
    }
  }

  if (single_acquisition) {
    SET_ADC_START_STOP(reg_cfg, START_SINGLE_CONVERSION);
  } else {
    SET_ADC_START_STOP(reg_cfg, START_CONTINUOUS_CONVERSION);
  }
  if (start) {

    uint16_t r = 0;

    write_direct_reg(device, OA_REG_LIVE_STATUS___SINGLE_PER_DEVICE, ADC_DATA_READY);

    do {
      write_direct_reg(device, OA_REG_ADC_CONV_CTRL___SINGLE_PER_DEVICE, reg_cfg);
      read_direct_reg(device, OA_REG_LIVE_STATUS___SINGLE_PER_DEVICE, r);
    } while ((r & ADC_BUSY_MASK) == 0);
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::stopAdc(bool power_down /*= false*/) {
  if (is_adc_started(OA_AN_DEVICE_0)) {
    stop_adc(OA_AN_DEVICE_0, power_down);
  }
#ifdef ARDUINO_OPTA_ANALOG
  if (is_adc_started(OA_AN_DEVICE_1)) {
    stop_adc(OA_AN_DEVICE_1, power_down);
  }

#endif
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::startAdc(bool single) {
  start_adc(0, single);
#ifdef ARDUINO_OPTA_ANALOG
  start_adc(1, single);
#endif
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::update_adc_value(uint8_t ch) {
  uint16_t read_value = 0;

  if (adc[ch].en_conversion) {

    if (read_reg(OA_REG_ADC_RESULT, read_value, ch)) {
      adc[ch].conversion = read_value;
      if (adc[ch].mov_average_req > 0) {
        if (adc[ch].average_samples < adc[ch].mov_average_req) {
          adc[ch].average_samples++;
        }
        if (adc[ch].average_samples == 1) {
          adc[ch].average = (double)read_value;
        } else {
          double v = adc[ch].average_samples;
          adc[ch].average -= (adc[ch].average / v);
          adc[ch].average += ((double)read_value / v);
        }
      }
    }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
bool OptaAnalog::is_adc_updatable(uint8_t device, bool wait_for_conversion) {
  bool rv = false;
  uint16_t r = 0;
  /* this function verify if the ADC data are ready to be read */

  if (is_adc_started(device)) {
    /* obviously it makes sense only if the ADC is started */
    if (wait_for_conversion) {
      /* if wait_for_conversion is true, then the function is blocking
       * and wait until the ADC has finished the conversion */
      do {
        read_direct_reg(device, OA_REG_LIVE_STATUS___SINGLE_PER_DEVICE, r);
        delay(1);
      } while ((r & ADC_DATA_READY) == 0);
      rv = true;
    } else {
      read_direct_reg(device, OA_REG_LIVE_STATUS___SINGLE_PER_DEVICE, r);
      if (r & ADC_DATA_READY) {
        rv = true;
        write_direct_reg(device, OA_REG_LIVE_STATUS___SINGLE_PER_DEVICE, ADC_DATA_READY);
      }
    }
  }
  return rv;
}

void OptaAnalog::updateAdc(bool wait_for_conversion /*= false*/) {

  if (is_adc_updatable(OA_AN_DEVICE_0, wait_for_conversion)) {
    update_adc_value(OA_CH_0);
    update_adc_value(OA_CH_1);
    update_adc_value(OA_CH_6);
    update_adc_value(OA_CH_7);
    write_direct_reg(OA_AN_DEVICE_0, OA_REG_LIVE_STATUS___SINGLE_PER_DEVICE, ADC_DATA_READY);
  }

#ifdef ARDUINO_OPTA_ANALOG
  if (is_adc_updatable(OA_AN_DEVICE_1, wait_for_conversion)) {
    update_adc_value(OA_CH_2);
    update_adc_value(OA_CH_3);
    update_adc_value(OA_CH_4);
    update_adc_value(OA_CH_5);
    write_direct_reg(OA_AN_DEVICE_1, OA_REG_LIVE_STATUS___SINGLE_PER_DEVICE, ADC_DATA_READY);
  }

#endif
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::update_adc_diagnostic(uint8_t ch) {
  uint16_t read_value = 0;
  if (adc[ch].en_conversion_diagnostic) {
    if (read_reg(OA_REG_DIAG_RESULT, read_value, ch)) {
      adc[ch].diag_conversion = read_value;
    }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::updateAdcDiagnostics() {
  /* READ ALL THE ADC CONVERSIONs FOR DEVICE 0 */
  if (is_adc_started(OA_AN_DEVICE_0)) {
    update_adc_diagnostic(OA_CH_0);
    update_adc_diagnostic(OA_CH_1);
    update_adc_diagnostic(OA_CH_6);
    update_adc_diagnostic(OA_CH_7);
  }
  /* READ ALL THE ADC CONVERSIONs FOR DEVICE 1 */

#ifdef ARDUINO_OPTA_ANALOG
  if (is_adc_started(OA_AN_DEVICE_1)) {
    update_adc_diagnostic(OA_CH_2);
    update_adc_diagnostic(OA_CH_3);
    update_adc_diagnostic(OA_CH_4);
    update_adc_diagnostic(OA_CH_5);
  }
#endif
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint16_t OptaAnalog::getAdcDiagValue(uint8_t ch) {
  if (ch < OA_AN_CHANNELS_NUM) {
    return adc[ch].diag_conversion;
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint16_t OptaAnalog::getAdcValue(uint8_t ch) {
  if (ch < OA_AN_CHANNELS_NUM) {
    return adc[ch].conversion;
  }
  return 0;
}

/* ##################################################################### */
/*                         RTD FUNCTIONs                                 */
/* ##################################################################### */

void OptaAnalog::configureRtd(uint8_t ch, bool use_3_w, float current_mA) {
  if (ch < OA_AN_CHANNELS_NUM) {
    rtd[ch].is_rtd = true;
    rtd[ch].set_measure_current(current_mA);

#ifdef ARDUINO_UNO_TESTALOG_SHIELD
    rtd[ch].use_3_wires = false;
    configureFunction(ch, CH_FUNC_RESISTANCE_MEASUREMENT);
#else
    if (ch == 0) {
      digitalWrite(DIO_RTD_SWITCH_1, HIGH);
    }
    if (ch == 1) {
      digitalWrite(DIO_RTD_SWITCH_2, HIGH);
    }

    if ((ch == 0 || ch == 1) && use_3_w) {
      rtd[ch].use_3_wires = true;
      configureFunction(ch, CH_FUNC_CURRENT_OUTPUT);
    } else {
      rtd[ch].use_3_wires = false;
      configureFunction(ch, CH_FUNC_RESISTANCE_MEASUREMENT);
    }
#endif
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

float OptaAnalog::getRtdValue(uint8_t ch) {
  if (ch < OA_AN_CHANNELS_NUM) {
    if (rtd[ch].is_rtd) {
      return (float)rtd[ch].RTD;
    }
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* in this function there are some call to the Module::update() function
 * this is not related to the update RTD function but is due to the fact
 * that the RTD update for 3 wire is extremely low (around 800 ms)
 * this could lead to problem in detecting a "reset" of other expansion
 * when they pull down the detect out pin so the Module::update is called
 * here */
void OptaAnalog::updateRtd() {

  // 3 wires RTD calculation
  Module::update();
  if ((rtd[0].is_rtd && rtd[0].use_3_wires) ||
      (rtd[1].is_rtd && rtd[1].use_3_wires)) {
#ifdef RTD_SET_SWTICH_AT_BEGIN
    // 1. put the switch in the LOW position
#ifdef ARDUINO_OPTA_ANALOG
    if (rtd[0].is_rtd && rtd[0].use_3_wires) {
      digitalWrite(DIO_RTD_SWITCH_1, LOW);
    }
    if (rtd[1].is_rtd && rtd[1].use_3_wires) {
      digitalWrite(DIO_RTD_SWITCH_2, LOW);
    }
#endif

    // 2. wait a little for the switch to be in the right position
    delay(10);
#endif

    // 3. put the current in to the DAC
    if (rtd[0].is_rtd && rtd[0].use_3_wires) {
      configureDacValue(0, rtd[0].current_value);
      updateDacValue(0, true);
    }
    if (rtd[1].is_rtd && rtd[1].use_3_wires) {
      configureDacValue(1, rtd[1].current_value);
      updateDacValue(1, true);
    }
    delay(2);
    Module::update();
    // 4. stop the ADC to program a new ADC configuration
    stopAdc();
    Module::update();

    // 5. program new ADC configuration
    if (rtd[0].is_rtd && rtd[0].use_3_wires) {
      configureAdcMux(0, CFG_ADC_INPUT_NODE_100OHM_R);
      configureAdcRange(0, CFG_ADC_RANGE_2_5V_LOOP);
      configureAdcPullDown(0, false);
      configureAdcRejection(0, true);
      configureAdcEnable(0, true);
      sendAdcConfiguration(0);
    }

    if (rtd[1].is_rtd && rtd[1].use_3_wires) {
      configureAdcMux(1, CFG_ADC_INPUT_NODE_100OHM_R);
      configureAdcRange(1, CFG_ADC_RANGE_2_5V_LOOP);
      configureAdcPullDown(1, false);
      configureAdcRejection(1, true);
      configureAdcEnable(1, true);
      sendAdcConfiguration(1);
    }

    // 6. start ADC
    startAdc();
    Module::update();
    // 7. update ADC value and wait for the conversion to be finished
    updateAdc(true);

    Module::update();
    // 8. store the measurement
    if (rtd[0].is_rtd && rtd[0].use_3_wires) {
      rtd[0].set_i_excite(adc[0].conversion);
    }
    if (rtd[1].is_rtd && rtd[1].use_3_wires) {
      rtd[1].set_i_excite(adc[1].conversion);
    }

    // 9. stop adc to program a new configuration
    stopAdc();
    Module::update();

    // 10. program new ADC configuration
    if (rtd[0].is_rtd && rtd[0].use_3_wires) {
      configureAdcMux(0, CFG_ADC_INPUT_NODE_IOP_AGND_SENSE);
      configureAdcRange(0, CFG_ADC_RANGE_2_5V_RTD); // range
      configureAdcPullDown(0, false);
      configureAdcRejection(0, true);
      configureAdcEnable(0, true);
      sendAdcConfiguration(0);
    }

    if (rtd[1].is_rtd && rtd[1].use_3_wires) {
      configureAdcMux(1, CFG_ADC_INPUT_NODE_IOP_AGND_SENSE);
      configureAdcRange(1, CFG_ADC_RANGE_2_5V_RTD); // range
      configureAdcPullDown(1, false);
      configureAdcRejection(1, true);
      configureAdcEnable(1, true);
      sendAdcConfiguration(1);
    }
    // 11. start ADC
    startAdc();
    Module::update();
    // 12. update ADC value and wait for the conversion to be finished
    updateAdc(true);

    // 13. store the measurement
    if (rtd[0].is_rtd && rtd[0].use_3_wires) {
      rtd[0].set_adc_RTD_2RL(adc[0].conversion);
    }
    if (rtd[1].is_rtd && rtd[1].use_3_wires) {
      rtd[1].set_adc_RTD_2RL(adc[1].conversion);
    }

// ???? set dac to 0 ??? before to change switch status
#ifdef RTD_CLEAR_DAC
    configureDacValue(ch, 0);
    updateDacValue(ch, true);
#endif

    // 14. change the position of the switch
#ifdef ARDUINO_OPTA_ANALOG
    if (rtd[0].is_rtd && rtd[0].use_3_wires) {
      digitalWrite(DIO_RTD_SWITCH_1, HIGH);
    }
    if (rtd[1].is_rtd && rtd[1].use_3_wires) {
      digitalWrite(DIO_RTD_SWITCH_2, HIGH);
    }
#endif

#ifdef RTD_CLEAR_DAC
    configureDacValue(ch, rtd[ch].current_value);
    updateDacValue(ch, true);
    delay(10);
#endif

    // 15. stop adc to program a new configuration
    stopAdc();

    Module::update();
    // 16. program new ADC configuration
    if (rtd[0].is_rtd && rtd[0].use_3_wires) {
      configureAdcMux(0, CFG_ADC_INPUT_NODE_100OHM_R);
      configureAdcRange(0, CFG_ADC_RANGE_2_5V_RTD);
      configureAdcPullDown(0, false);
      configureAdcRejection(0, true);
      configureAdcEnable(0, true);
      sendAdcConfiguration(0);
    }

    if (rtd[1].is_rtd && rtd[1].use_3_wires) {
      configureAdcMux(1, CFG_ADC_INPUT_NODE_100OHM_R);
      configureAdcRange(1, CFG_ADC_RANGE_2_5V_RTD);
      configureAdcPullDown(1, false);
      configureAdcRejection(1, true);
      configureAdcEnable(1, true);
      sendAdcConfiguration(1);
    }

    // NOTE: RTD 3 wires calculation continues here below
    // outside the if 3 wires (this is wanted and is because
    // the last steps are common to 2 wires calculation )

  } // 3 wires RTD calculation if

  if (rtd[0].is_rtd || rtd[1].is_rtd || rtd[2].is_rtd || rtd[3].is_rtd ||
      rtd[4].is_rtd || rtd[5].is_rtd || rtd[6].is_rtd || rtd[7].is_rtd) {
    // (17. from 3 wires) start ADC
    startAdc();
    // (18. from 3 wires) update ADC value and wait for the conversion to be
    // finished
    updateAdc(true);

    Module::update();
    // (19. from 3 wires) store the measurement
    if (rtd[0].is_rtd && rtd[0].use_3_wires) {
      rtd[0].set_adc_RTD_RL(adc[0].conversion);
      rtd[0].calc_RTD();
    } else if (rtd[0].is_rtd) {
      rtd[0].set(adc[0].conversion);
    }

    if (rtd[1].is_rtd && rtd[1].use_3_wires) {
      rtd[1].set_adc_RTD_RL(adc[1].conversion);
      rtd[1].calc_RTD();
    } else if (rtd[1].is_rtd) {
      rtd[1].set(adc[1].conversion);
    }
    if (rtd[2].is_rtd) {
      rtd[2].set(adc[2].conversion);
    }
    if (rtd[3].is_rtd) {
      rtd[3].set(adc[3].conversion);
    }
    if (rtd[4].is_rtd) {
      rtd[4].set(adc[4].conversion);
    }
    if (rtd[5].is_rtd) {
      rtd[5].set(adc[5].conversion);
    }
    if (rtd[6].is_rtd) {
      rtd[6].set(adc[6].conversion);
    }
    if (rtd[7].is_rtd) {
      rtd[7].set(adc[7].conversion);
    }
    Module::update();
  }

  // this section of 3 wires RTD is just to reset the current and turn
  // the switch in LOW position again
  if (rtd[0].is_rtd || rtd[1].is_rtd) {
    // 1. put the switch in the LOW position
#ifdef ARDUINO_OPTA_ANALOG
    if (rtd[0].is_rtd) {
      configureDacValue(0, 0);
      updateDacValue(0, true);
      delay(1);
      digitalWrite(DIO_RTD_SWITCH_1, LOW);
    }
    if (rtd[1].is_rtd) {
      configureDacValue(1, 0);
      updateDacValue(1, true);
      delay(1);
      digitalWrite(DIO_RTD_SWITCH_2, LOW);
    }
#endif
  }
  Module::update();
}

/* ###################################################################### */
/*                         DAC FUNCTIONs                                  */
/* ###################################################################### */

void OptaAnalog::configureDacCurrLimit(uint8_t ch, CfgOutCurrLim_t cl) {
  if (ch < OA_AN_CHANNELS_NUM) {
    dac[ch].current_limit = cl;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureDacUseSlew(uint8_t ch, CfgOutSlewRate_t sr,
                                     CfgOutSlewStep_t r) {
  if (ch < OA_AN_CHANNELS_NUM) {
    dac[ch].enable_slew = true;
    dac[ch].slew_rate = sr;
    dac[ch].slew_step = r;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureDacDisableSlew(uint8_t ch) {
  if (ch < OA_AN_CHANNELS_NUM) {
    dac[ch].enable_slew = false;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureDacUseReset(uint8_t ch, uint16_t value) {
  if (ch < OA_AN_CHANNELS_NUM) {
    dac[ch].enable_clear = true;
    dac[ch].reset_value = value;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureDacDisableReset(uint8_t ch) {
  if (ch < OA_AN_CHANNELS_NUM) {
    dac[ch].enable_clear = false;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::sendDacConfiguration(uint8_t ch) {
  uint16_t cfg = 0;
  if (ch < OA_AN_CHANNELS_NUM) {

    if (dac[ch].current_limit == OUT_CURRENT_LIMIT_7_5mA) {
      cfg |= CURRENT_LIMIT_7_5mA;
    } else { /* nothing to do */
    }

    if (dac[ch].enable_clear) {
      ENABLE_CLEAR(cfg);
    } else { /* nothing to do */
    }

    if (dac[ch].enable_slew) {
      ENABLE_SLEW(cfg);
    } else { /* nothing to do */
    }

    switch (dac[ch].slew_rate) {
    case OUT_SLEW_RATE_4k:
      SET_SLEW_RATE(cfg, SLEW_RATE_4k);
      break;
    case OUT_SLEW_RATE_64k:
      SET_SLEW_RATE(cfg, SLEW_RATE_64k);
      break;
    case OUT_SLEW_RATE_150k:
      SET_SLEW_RATE(cfg, SLEW_RATE_150k);
      break;
    case OUT_SLEW_RATE_240k:
      SET_SLEW_RATE(cfg, SLEW_RATE_240k);
      break;
    default:
      SET_SLEW_RATE(cfg, SLEW_RATE_4k);
      break;
    }

    switch (dac[ch].slew_step) {
    case OUT_SLEW_STEP_64:
      SET_SLEW_STEP(cfg, SLEW_STEP_64);
      break;
    case OUT_SLEW_STEP_120:
      SET_SLEW_STEP(cfg, SLEW_STEP_120);
      break;
    case OUT_SLEW_STEP_500:
      SET_SLEW_STEP(cfg, SLEW_STEP_500);
      break;
    case OUT_SLEW_STEP_1820:
      SET_SLEW_STEP(cfg, SLEW_STEP_1820);
      break;
    default:
      SET_SLEW_STEP(cfg, SLEW_STEP_64);
      break;
    }
    write_reg(OA_REG_DAC_CONFIG, cfg, ch);
    if (dac[ch].enable_clear) {
      write_reg(OA_REC_DAC_CLEAR_CODE, dac[ch].reset_value, ch);
    }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureDacValue(uint8_t ch, uint16_t value) {
  if (ch < OA_AN_CHANNELS_NUM) {
    if (value <= OA_MAX_DAC_VALUE) {
      dac_values[ch] = value;
    }
  }
}
void OptaAnalog::toggle_ldac() {
  digitalWrite(LDAC1, LOW);
  digitalWrite(LDAC2, LOW);
  delay(5);
  digitalWrite(LDAC1, HIGH);
  digitalWrite(LDAC2, HIGH);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::reset_dac_value(uint8_t ch) {
  if (ch < OA_AN_CHANNELS_NUM) {
    write_reg(OA_REG_DAC_CODE, 0, ch);
    uint16_t set_value = 0;
    read_reg(OA_REG_DAC_CODE, set_value, ch);
    while(set_value != 0) {
      write_reg(OA_REG_DAC_CODE, 0, ch);
      read_reg(OA_REG_DAC_CODE, set_value, ch);
    }
    toggle_ldac();
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::write_dac_defaults(uint8_t ch) {
  if (ch < OA_AN_CHANNELS_NUM) {
    write_reg(OA_REG_DAC_CODE, dac_defaults[ch], ch);
    uint16_t set_value = 0;
    read_reg(OA_REG_DAC_CODE, set_value, ch);
    while(set_value != dac_defaults[ch]) {
      write_reg(OA_REG_DAC_CODE, dac_defaults[ch], ch);
      read_reg(OA_REG_DAC_CODE, set_value, ch);
    }
  }
}


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::updateDacValue(uint8_t ch, bool toggle /*= true*/) {
  if (ch < OA_AN_CHANNELS_NUM) {
      write_reg(OA_REG_DAC_CODE, dac_values[ch], ch);
      uint16_t set_value = 0;
      read_reg(OA_REG_DAC_CODE, set_value, ch);
      while(set_value != dac_values[ch]) {
        write_reg(OA_REG_DAC_CODE, dac_values[ch], ch);
        read_reg(OA_REG_DAC_CODE, set_value, ch);
      }
      if(!dac_value_updated[ch]) {
        dac_value_updated[ch] = true;
      }
      
      if (toggle) {
        toggle_ldac();
      }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::resetDacValue(uint8_t ch) {
  // TODO: implement
  (void)ch;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool OptaAnalog::is_dac_used(uint8_t ch) {
  if (ch < OA_AN_CHANNELS_NUM) {
    if(rtd[ch].is_rtd == true) {
      /* RTD 3 wires uses CURRENT OUTPUT but is not a DAC function 
         this has to deal with default values */
      return false;
    }

    return (fun[ch] == CH_FUNC_VOLTAGE_OUTPUT ||
            fun[ch] == CH_FUNC_CURRENT_OUTPUT);
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::updateDac(uint8_t ch) {
  if (is_dac_used(ch)) {
    updateDacPresentValue(ch);
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::updateDacPresentValue(uint8_t ch) {
  if (ch < OA_AN_CHANNELS_NUM) {
    /* Opta Analog DAC ACTIVE code (0x1E -> 0x21) */
    read_reg(OA_REG_DAC_ACTIVE, dac[ch].present_value, ch);
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint16_t OptaAnalog::getDacCurrentValue(uint8_t ch) {
  if (ch < OA_AN_CHANNELS_NUM) {
    return dac[ch].present_value;
  }
  return 0;
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* ######################################################################## */
/*                            DIN FUNCTIONs */
/* ######################################################################## */

void OptaAnalog::configureDinFilterCompIn(uint8_t ch, bool en) {
  if (ch < OA_AN_CHANNELS_NUM) {
    din[ch].filter_input_comparator = en;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureDinInvertCompOut(uint8_t ch, bool en) {
  if (ch < OA_AN_CHANNELS_NUM) {
    din[ch].invert_comparator_output = en;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureDinEnableComp(uint8_t ch, bool en) {
  if (ch < OA_AN_CHANNELS_NUM) {
    din[ch].enable_comparator = en;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureDinDebounceSimple(uint8_t ch, bool en) {
  if (ch < OA_AN_CHANNELS_NUM) {
    din[ch].debounce_mode_simple = en;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureDinScaleComp(uint8_t ch, bool en) {
  uint8_t device = GET_DEVICE_FROM_CHANNEL(ch);
  di_scaled[device] = en;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureDinCompTh(uint8_t ch, uint8_t v) {
  if (v >= 32) { // only 5 bits available
    v = 31;
  }
  uint8_t device = GET_DEVICE_FROM_CHANNEL(ch);
  di_th[device] = v;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureDinCurrentSink(uint8_t ch, uint8_t v) {
  if (v >= 16) { // only 4 bits available
    v = 15;
  }
  if (ch < OA_AN_CHANNELS_NUM) {
    din[ch].current_sink = v;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureDinDebounceTime(uint8_t ch, uint8_t v) {
  if (v >= 32) { // only 5 bits available
    v = 31;
  }
  if (ch < OA_AN_CHANNELS_NUM) {
    din[ch].debounce_time = v;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::sendDinConfiguration(uint8_t ch) {
  uint16_t reg_cfg = 0;
  if (ch < OA_AN_CHANNELS_NUM) {
    if (din[ch].filter_input_comparator) {
      FILTER_COMPARATOR(reg_cfg);
    }

    if (din[ch].invert_comparator_output) {
      INVERT_COMPARATOR(reg_cfg);
    }

    if (din[ch].enable_comparator) {
      ENABLE_COMPARATOR(reg_cfg);
    }

    reg_cfg |= (din[ch].current_sink << DIN_SINK_BIT_START);

    if (din[ch].debounce_mode_simple) {
      SET_DEBOUNCE_MODE_COUNTER(reg_cfg);
    }

    DEBOUNCE_TIME(reg_cfg, din[ch].debounce_time);

    write_reg(OA_REG_DIN_CONFIG, reg_cfg, ch);

    reg_cfg = 0;

    uint8_t device = GET_DEVICE_FROM_CHANNEL(ch);

    if (!di_scaled[device]) {
      reg_cfg = TH_FIXED_TO_16V;
    }

    COMPARATOR_TH(reg_cfg, di_th[device]);

    write_direct_reg(device, OA_REG_DIN_THRESH___SINGLE_PER_DEVICE, reg_cfg);
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::updateDinReadings() {
  uint16_t read_value = 0;
  digital_ins = 0;
  /* since channel 0 belongs to the first device we use
   * this trick to use get_device function on channel 0
   */

  read_direct_reg(OA_AN_DEVICE_0, OA_REG_DIN_COMP_OUT___SINGLE_PER_DEVICE, read_value);
#ifdef ARDUINO_UNO_TESTALOG_SHIELD
  digital_ins |= (read_value & 0x0F);
#else
  digital_ins |= (read_value & 0x2) >> 1;
  digital_ins |= (read_value & 0x1) << 1;
  digital_ins |= (read_value & 0xC) << 4;
#endif
  /* since channel 2 belongs to the second device we
   * use this trick to use get_device function on
   * channel 2 */

  read_direct_reg(OA_AN_DEVICE_1, OA_REG_DIN_COMP_OUT___SINGLE_PER_DEVICE, read_value);
  digital_ins |= (read_value & 0xF) << 2;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool OptaAnalog::getDinValue(uint8_t ch) {
  if (ch < OA_AN_CHANNELS_NUM) {
    if (digital_ins & (1 << ch)) {
      return true;
    }
  }
  return false;
}

/* ####################################################################### */
/*                         GPO FUNCTIONs                                   */
/* ####################################################################### */

/* GPO FUNCTIONS ARE AVAILABLE ONLY FOR TestaLog ana not for OPTA ANALOG */

void OptaAnalog::configureGpo(uint8_t ch, CfgGpo_t f, GpoState_t state) {
#ifndef ARDUINO_OPTA_ANALOG
  if (ch < OA_GPO_NUM) {
    gpo[ch].cfg = f;
    gpo[ch].state = state;
  }
#else
  (void)ch;
  (void)f;
  (void)state;
#endif
}

/* CONFIGURE the GPO function for the CHANNEL ch */
void OptaAnalog::updateGpo(uint8_t ch) {
#ifndef ARDUINO_OPTA_ANALOG
  uint16_t cfg = 0;
  if (ch < OA_GPO_NUM) {
    switch (gpo[ch].cfg) {
    case GPO_PULL_DOWN:
      cfg = GPO_SELECT_PULL_DOWN;
      break;
    case GPO_DRIVEN_SINGLE:
      cfg = GPO_SELECT_DRIVE_VIA_CONFIG;
      break;
    case GPO_DRIVEN_PARALLEL:
      cfg = GPO_SELECT_DRIVE_VIA_PARALLEL;
      break;
    case GPO_OUT_DEB_COMP:
      cfg = GPO_SELECT_DEBOUNCE_COMP;
      break;
    case GPO_HIGH_IMP:
      cfg = GPO_SELECT_HIGH_IMPEDENCE;
      break;
    default:
      cfg = GPO_SELECT_HIGH_IMPEDENCE;
      break;
    }
    if (gpo[ch].state == GPO_HIGH) {
      GPO_SELECT_HIGH(cfg);
    } else {
      /* nothing to do, already reset */
    }
    write_reg(OA_REG_GPO_CONFIG, cfg, ch);
  }
#else
  (void)ch;
#endif
}

void OptaAnalog::digitalWriteAnalog(uint8_t ch, GpoState_t s) {
#ifndef ARDUINO_OPTA_ANALOG
  if (ch < OA_GPO_NUM) {
    gpo[ch].cfg = GPO_DRIVEN_SINGLE;
    gpo[ch].state = s;
  }
  updateGpo(ch);
#else
  (void)ch;
  (void)s;
#endif
}



void OptaAnalog::swAnalogDevReset() {
#ifdef ARDUINO_OPTA_ANALOG
  stop_adc(0, false);
  stop_adc(1, false);
  digitalWrite(DIO_RESET_1, LOW);
  digitalWrite(DIO_RESET_2, LOW);
  delay(50);
  digitalWrite(DIO_RESET_1, HIGH);
  digitalWrite(DIO_RESET_2, HIGH);
  delay(100);
  adc_ch_mask_0 = 0;
  adc_ch_mask_1 = 0;
  adc_ch_mask_0_last = 0;
  adc_ch_mask_1_last = 0;
#else
  digitalWrite(DIO_RESET_1, LOW);
  delay(50);
  digitalWrite(DIO_RESET_1, HIGH);
  delay(100);
#endif
  for (int i = 0; i < OA_AN_CHANNELS_NUM; i++) {
    fun[i] = CH_FUNC_HIGH_IMPEDENCE;
    adc[i].en_conversion = false;
    rtd[i].is_rtd = false;
    rtd[i].use_3_wires = false;
  }
}

void OptaAnalog::sychLDAC() {
#ifdef ARDUINO_OPTA_ANALOG
  write_direct_reg(OA_AN_DEVICE_0, OA_REG_CMD_REGISTER___SINGLE_PER_DEVICE, OPTA_AN_KEY_LDAC);
  write_direct_reg(OA_AN_DEVICE_1, OA_REG_CMD_REGISTER___SINGLE_PER_DEVICE, OPTA_AN_KEY_LDAC);
#else
  write_direct_reg(OA_AN_DEVICE_0, OA_REG_CMD_REGISTER___SINGLE_PER_DEVICE, OPTA_AN_KEY_LDAC);
#endif
}


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::update_live_status() {
  read_direct_reg(OA_AN_DEVICE_0, OA_REG_LIVE_STATUS___SINGLE_PER_DEVICE, live_state[OA_AN_DEVICE_0]);
  read_direct_reg(OA_AN_DEVICE_1, OA_REG_LIVE_STATUS___SINGLE_PER_DEVICE, live_state[OA_AN_DEVICE_1]);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::update_alert_mask(int8_t ch) {
  // TODO: implement
  (void)ch;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::update_alert_status() {
  read_direct_reg(OA_AN_DEVICE_0, OA_REG_ALERT_STATUS___SINGLE_PER_DEVICE, alert_state[OA_AN_DEVICE_0]);
  read_direct_reg(OA_AN_DEVICE_1, OA_REG_ALERT_STATUS___SINGLE_PER_DEVICE, alert_state[OA_AN_DEVICE_1]);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void OptaAnalog::configureAlertMaskRegister(uint8_t device, uint16_t alert) {
  // TODO: implement
  (void)device;
  (void)alert;
}

/* ####################################################################### */
/*                     PARSING FUNCTIONs                                   */
/* ####################################################################### */

bool OptaAnalog::parse_setup_di_channel() {
  if (checkSetMsgReceived(rx_buffer, ARG_OA_CH_DI, LEN_OA_CH_DI)) {
    uint8_t ch = rx_buffer[OA_CH_DI_CHANNEL_POS];
    
    if(ch >= OA_AN_CHANNELS_NUM) {
      return true;
    }

    if (ch < OA_AN_CHANNELS_NUM) {
      rtd[ch].is_rtd = false;
    }

    configureFunction(ch, CH_FUNC_DIGITAL_INPUT);
    if (rx_buffer[OA_CH_DI_FILTER_COMP_POS] == OA_ENABLE) {
      configureDinFilterCompIn(ch, true);
    } else {
      configureDinFilterCompIn(ch, false);
    }
    if (rx_buffer[OA_CH_DI_INVERT_COMP_POS] == OA_ENABLE) {
      configureDinInvertCompOut(ch, true);
    } else {
      configureDinInvertCompOut(ch, false);
    }
    if (rx_buffer[OA_CH_DI_ENABLE_COMP_POS] == OA_ENABLE) {
      configureDinEnableComp(ch, true);
    } else {
      configureDinEnableComp(ch, false);
    }
    if (rx_buffer[OA_CH_DI_DEBOUNCE_SIMPLE_POS] == OA_ENABLE) {
      configureDinDebounceSimple(ch, true);
    } else {
      configureDinDebounceSimple(ch, false);
    }
    if (rx_buffer[OA_CH_DI_SCALE_COMP_POS] == OA_ENABLE) {
      configureDinScaleComp(ch, true);
    } else {
      configureDinScaleComp(ch, false);
    }
    configureDinCompTh(ch, rx_buffer[OA_CH_DI_COMP_TH_POS]);

    configureDinCurrentSink(ch, rx_buffer[OA_CH_DI_CURR_SINK_POS]);
    configureDinDebounceTime(ch, rx_buffer[OA_CH_DI_DEBOUNCE_TIME_POS]);

    prepareSetAns(tx_buffer, ANS_ARG_OA_ACK, ANS_LEN_OA_ACK);

    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool OptaAnalog::parse_setup_dac_channel() {
  if (checkSetMsgReceived(rx_buffer, ARG_OA_CH_DAC, LEN_OA_CH_DAC)) {
    uint8_t ch = rx_buffer[OA_CH_DAC_CHANNEL_POS];

    if(ch >= OA_AN_CHANNELS_NUM) {
      return true;
    }

    if (ch < OA_AN_CHANNELS_NUM) {
      rtd[ch].is_rtd = false;
    }

    if (rx_buffer[OA_CH_DAC_TYPE_POS] == OA_VOLTAGE_DAC) {
      configureFunction(ch, CH_FUNC_VOLTAGE_OUTPUT);
    } else if (rx_buffer[OA_CH_DAC_TYPE_POS] == OA_CURRENT_DAC) {
      configureFunction(ch, CH_FUNC_CURRENT_OUTPUT);
    }

    if (rx_buffer[OA_CH_DAC_LIMIT_CURRENT_POS] == OA_ENABLE) {
      configureDacCurrLimit(ch, OUT_CURRENT_LIMIT_7_5mA);

    } else {
      configureDacCurrLimit(ch, OUT_CURRENT_LIMIT_30mA);
    }
    if (rx_buffer[OA_CH_DAC_ENABLE_SLEW_POS] == OA_ENABLE) {
      switch (rx_buffer[OA_CH_DAC_SLEW_RATE_POS]) {
      case 0: // OA_SLEW_RATE_0:
        configureDacUseSlew(ch, OUT_SLEW_RATE_4k, OUT_SLEW_STEP_64);
        break;
      case 1: // OA_SLEW_RATE_1:
        configureDacUseSlew(ch, OUT_SLEW_RATE_4k, OUT_SLEW_STEP_120);
        break;
      case 2: // OA_SLEW_RATE_2:
        configureDacUseSlew(ch, OUT_SLEW_RATE_4k, OUT_SLEW_STEP_500);
        break;
      case 3: // OA_SLEW_RATE_3:
        configureDacUseSlew(ch, OUT_SLEW_RATE_4k, OUT_SLEW_STEP_1820);
        break;
      case 4: // OA_SLEW_RATE_4:
        configureDacUseSlew(ch, OUT_SLEW_RATE_64k, OUT_SLEW_STEP_64);
        break;
      case 5: // OA_SLEW_RATE_5:
        configureDacUseSlew(ch, OUT_SLEW_RATE_64k, OUT_SLEW_STEP_120);
        break;
      case 6: // OA_SLEW_RATE_6:
        configureDacUseSlew(ch, OUT_SLEW_RATE_64k, OUT_SLEW_STEP_500);
        break;
      case 7: // OA_SLEW_RATE_7:
        configureDacUseSlew(ch, OUT_SLEW_RATE_64k, OUT_SLEW_STEP_1820);
        break;
      case 8: // OA_SLEW_RATE_8:
        configureDacUseSlew(ch, OUT_SLEW_RATE_150k, OUT_SLEW_STEP_64);
        break;
      case 9: // OA_SLEW_RATE_9:
        configureDacUseSlew(ch, OUT_SLEW_RATE_150k, OUT_SLEW_STEP_120);
        break;
      case 10: // OA_SLEW_RATE_10:
        configureDacUseSlew(ch, OUT_SLEW_RATE_150k, OUT_SLEW_STEP_500);
        break;
      case 11: // OA_SLEW_RATE_11:
        configureDacUseSlew(ch, OUT_SLEW_RATE_150k, OUT_SLEW_STEP_1820);
        break;
      case 12: // OA_SLEW_RATE_12:
        configureDacUseSlew(ch, OUT_SLEW_RATE_240k, OUT_SLEW_STEP_64);
        break;
      case 13: // OA_SLEW_RATE_13:
        configureDacUseSlew(ch, OUT_SLEW_RATE_240k, OUT_SLEW_STEP_120);
        break;
      case 14: // OA_SLEW_RATE_14:
        configureDacUseSlew(ch, OUT_SLEW_RATE_240k, OUT_SLEW_STEP_500);
        break;
      case 15: // OA_SLEW_RATE_15:
        configureDacUseSlew(ch, OUT_SLEW_RATE_240k, OUT_SLEW_STEP_1820);
        break;
      }
    } else {
      configureDacDisableSlew(ch);
    }
    prepareSetAns(tx_buffer, ANS_ARG_OA_ACK, ANS_LEN_OA_ACK);
    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool OptaAnalog::parse_setup_rtd_channel() {
  if (checkSetMsgReceived(rx_buffer, ARG_OA_CH_RTD, LEN_OA_CH_RTD)) {
    uint8_t ch = rx_buffer[OA_CH_RTD_CHANNEL_POS];

    if(ch >= OA_AN_CHANNELS_NUM) {
      return true;
    }

    Float_u v;
    for (int i = 0; i < 4; i++) {
      v.bytes[i] = rx_buffer[OA_CH_RTD_CURRENT_POS + i];
    }
    if (rx_buffer[OA_CH_RTD_3WIRE_POS] == OA_ENABLE) {
      configureRtd(ch, true, v.value);
    } else {
      configureRtd(ch, false, v.value);
    }
    prepareSetAns(tx_buffer, ANS_ARG_OA_ACK, ANS_LEN_OA_ACK);
    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool OptaAnalog::parse_setup_high_imp_channel() {
  if (checkSetMsgReceived(rx_buffer, ARG_OA_CH_HIGH_IMPEDENCE,
                          LEN_OA_CH_HIGH_IMPEDENCE)) {
    uint8_t ch = rx_buffer[OA_HIGH_IMPEDENCE_CH_POS];

    if(ch >= OA_AN_CHANNELS_NUM) {
      return true;
    }
    rtd[ch].is_rtd = false;
    configureFunction(ch, CH_FUNC_HIGH_IMPEDENCE);
    prepareSetAns(tx_buffer, ANS_ARG_OA_ACK, ANS_LEN_OA_ACK);
    return true;
  }
  return false;
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool OptaAnalog::parse_setup_adc_channel() {
  if (checkSetMsgReceived(rx_buffer, ARG_OA_CH_ADC, LEN_OA_CH_ADC)) {

    uint8_t ch = rx_buffer[OA_CH_ADC_CHANNEL_POS];
    
    if (ch >= OA_AN_CHANNELS_NUM) {
       return true;
    }
    bool write = true;
    if (rx_buffer[OA_CH_ADC_ADDING_ADC_POS] == OA_ENABLE) {
      write = false;
    }

    rtd[ch].is_rtd = false;

    CfgFun_t tmp_f = CH_FUNC_HIGH_IMPEDENCE;

    if(update_fun[ch].size() > 0) {
      tmp_f = update_fun[ch].back().f;
    }

    /* cannot wait for the fun[ch] to be updated if the add ADC is done immediately
     * after the begin DAC because it can take some times to update the channel
     * configuration. With this change we ensure that the last channel
     * configuration used is immediately checked */
    if ( (tmp_f == CH_FUNC_VOLTAGE_OUTPUT || fun[ch] == CH_FUNC_VOLTAGE_OUTPUT) &&
        write == false &&
        rx_buffer[OA_CH_ADC_TYPE_POS] == OA_CURRENT_ADC) {
      /* this is a special case:
       * we have DAC voltage output on that channel but we are adding an ADC
       * current measurement on the same channel */

      /* note that the function here is not really used since it will
      not sent to the Analog Device chip (write_function_configuration[ch]
      is false indeed) */
      configureFunction(ch, CH_FUNC_CURRENT_INPUT_LOOP_POWER, write);
      configureAdcMux(ch, CFG_ADC_INPUT_NODE_100OHM_R);
      configureAdcRange(ch, CFG_ADC_RANGE_2_5V_BI);
      configureAdcPullDown(ch, false);
    } else if (rx_buffer[OA_CH_ADC_TYPE_POS] == OA_VOLTAGE_ADC) {
      configureFunction(ch, CH_FUNC_VOLTAGE_INPUT,write);
      configureAdcMux(ch, CFG_ADC_INPUT_NODE_IOP_AGND_SENSE);
      configureAdcRange(ch, CFG_ADC_RANGE_10V);
      if (rx_buffer[OA_CH_ADC_PULL_DOWN_POS] == OA_ENABLE) {
        configureAdcPullDown(rx_buffer[OA_CH_ADC_CHANNEL_POS], true);
      } else if (rx_buffer[OA_CH_ADC_PULL_DOWN_POS] == OA_DISABLE) {
        configureAdcPullDown(rx_buffer[OA_CH_ADC_CHANNEL_POS], false);
      }
    } else if (rx_buffer[OA_CH_ADC_TYPE_POS] == OA_CURRENT_ADC) {
      configureFunction(ch, CH_FUNC_CURRENT_INPUT_EXT_POWER, write);
      configureAdcMux(ch, CFG_ADC_INPUT_NODE_100OHM_R);
      configureAdcRange(ch, CFG_ADC_RANGE_2_5V_RTD);
      configureAdcPullDown(ch, false);
    }

    if (rx_buffer[OA_CH_ADC_REJECTION_POS] == OA_ENABLE) {
      configureAdcRejection(ch, true);
      configureAdcDiagRejection(ch, true);
    } else if (rx_buffer[OA_CH_ADC_REJECTION_POS] == OA_DISABLE) {
      configureAdcRejection(ch, false);
      configureAdcDiagRejection(ch, false);
    }
    if (rx_buffer[OA_CH_ADC_DIAGNOSTIC_POS] == OA_ENABLE) {
      configureAdcDiagnostic(ch, true);
    } else if (rx_buffer[OA_CH_ADC_DIAGNOSTIC_POS] == OA_DISABLE) {
      configureAdcDiagnostic(ch, false);
    }
    configureAdcMovingAverage(ch, rx_buffer[OA_CH_ADC_MOVING_AVE_POS]);

    prepareSetAns(tx_buffer, ANS_ARG_OA_ACK, ANS_LEN_OA_ACK);

    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool OptaAnalog::parse_get_rtd_value() {
  if (checkGetMsgReceived(rx_buffer, ARG_OA_GET_RTD, LEN_OA_GET_RTD)) {
    uint8_t ch = rx_buffer[OA_CH_RTD_CHANNEL_POS];

    if(ch >= OA_AN_CHANNELS_NUM) {
      ch = 0;
    }

    tx_buffer[ANS_OA_GET_RTD_CHANNEL_POS] = ch;
    if (ch < OA_AN_CHANNELS_NUM) {
      Float_u _rtd;
      _rtd.value = rtd[ch].RTD;
      for (int i = 0; i < 4; i++) {
        tx_buffer[ANS_OA_GET_RTD_VALUE_POS + i] = _rtd.bytes[i];
      }
    } else {
      for (int i = 0; i < 4; i++) {
        tx_buffer[ANS_OA_GET_RTD_VALUE_POS + i] = 0;
      }
    }
    prepareGetAns(tx_buffer, ANS_ARG_OA_GET_RTD, ANS_LEN_OA_GET_RTD);
    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool OptaAnalog::parse_set_rtd_update_rate() {
  bool rv = false;
  uint16_t rate = rx_buffer[OA_SET_RTD_UPDATE_TIME_POS];
  rate += (rx_buffer[OA_SET_RTD_UPDATE_TIME_POS + 1] << 8);
  if (checkSetMsgReceived(rx_buffer, ARG_OA_SET_RTD_UPDATE_TIME,
                          LEN_OA_SET_RTD_UPDATE_TIME)) {
    
    rtd_update_time = rate;
    rv = true;
  }
  else if(checkSetMsgReceived(rx_buffer, ARG_OA_SET_TIMEOUT_TIME,
                          LEN_OA_SET_RTD_UPDATE_TIME)) {
    
    timer_timout_ms = rate;
    rv = true;

  }
  if(rv) {
    prepareSetAns(tx_buffer, ANS_ARG_OA_ACK, ANS_LEN_OA_ACK);
  }

  return rv;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool OptaAnalog::parse_set_dac_value() {
  if (checkSetMsgReceived(rx_buffer, ARG_OA_SET_DAC, LEN_OA_SET_DAC)) {

    uint8_t ch = rx_buffer[OA_SET_DAC_CHANNEL_POS];

    if(ch >= OA_AN_CHANNELS_NUM) {
      return true;
    }

    uint16_t value = rx_buffer[OA_SET_DAC_VALUE_POS];
    value += (rx_buffer[OA_SET_DAC_VALUE_POS + 1] << 8);

    configureDacValue(ch, value);
    dac_value_updated[ch] = false;
    
    if (rx_buffer[OA_SET_DAC_UPDATE_VALUE] == 1) {
      update_dac_using_LDAC = true;
    } 

    /* value are sent to the analog device during
     * update function */
    prepareSetAns(tx_buffer, ANS_ARG_OA_ACK, ANS_LEN_OA_ACK);
    return true;
  }
  else if(checkSetMsgReceived(rx_buffer, ARG_OA_SET_DAC_DEFAULT,LEN_OA_SET_DAC)) {
    uint8_t ch = rx_buffer[OA_SET_DAC_CHANNEL_POS];

    if(ch >= OA_AN_CHANNELS_NUM) {
      return true;
    }

    uint16_t value = rx_buffer[OA_SET_DAC_VALUE_POS];
    value += (rx_buffer[OA_SET_DAC_VALUE_POS + 1] << 8);
    dac_defaults[ch] = value;
    prepareSetAns(tx_buffer, ANS_ARG_OA_ACK, ANS_LEN_OA_ACK);
    return true;

  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool OptaAnalog::parse_set_all_dac_value() {

  if (checkSetMsgReceived(rx_buffer, ARG_OA_SET_ALL_DAC, LEN_OA_SET_ALL_DAC)) {

    update_dac_using_LDAC = true;
    prepareSetAns(tx_buffer, ANS_ARG_OA_ACK, ANS_LEN_OA_ACK);
    return true;
  }
  return false;
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool OptaAnalog::parse_get_adc_value() {
  if (checkGetMsgReceived(rx_buffer, ARG_OA_GET_ADC, LEN_OA_GET_ADC)) {
    uint8_t ch = rx_buffer[OA_CH_ADC_CHANNEL_POS];

    if(ch >= OA_AN_CHANNELS_NUM) {
      ch = 0;
    }

    tx_buffer[ANS_OA_ADC_CHANNEL_POS] = ch;
    if (ch < OA_AN_CHANNELS_NUM) {
      uint16_t value = 0;
      if (adc[ch].mov_average_req > 0) {
        value = (uint16_t)adc[ch].average;
      } else {
        value = adc[ch].conversion;
      }

      tx_buffer[ANS_OA_ADC_VALUE_POS] = (uint8_t)(value & 0xFF);
      tx_buffer[ANS_OA_ADC_VALUE_POS + 1] = (uint8_t)((value & 0xFF00) >> 8);
    } else {
      tx_buffer[ANS_OA_ADC_VALUE_POS] = 0;
      tx_buffer[ANS_OA_ADC_VALUE_POS + 1] = 0;
    }
    prepareGetAns(tx_buffer, ANS_ARG_OA_GET_ADC, ANS_LEN_OA_GET_ADC);
    return true;
  }
  return false;
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool OptaAnalog::parse_get_all_adc_value() {
  if (checkGetMsgReceived(rx_buffer, ARG_OA_GET_ALL_ADC, LEN_OA_GET_ALL_ADC)) {

    const int s = ANS_OA_ADC_GET_ALL_VALUE_POS;
    for (int ch = 0; ch < OA_AN_CHANNELS_NUM; ch++) {

      uint16_t value = 0;
      if (adc[ch].mov_average_req > 0) {
        value = (uint16_t)adc[ch].average;

      } else {
        value = adc[ch].conversion;
      }

      tx_buffer[s + 2 * ch] = (uint8_t)(value & 0xFF);
      tx_buffer[s + 2 * ch + 1] = (uint8_t)((value & 0xFF00) >> 8);
    }

    prepareGetAns(tx_buffer, ANS_ARG_OA_GET_ALL_ADC, ANS_LEN_OA_GET_ALL_ADC);
    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool OptaAnalog::parse_set_pwm_value() {
  bool rv = false;
  uint8_t ch = rx_buffer[OA_SET_PWM_CHANNEL_POS];
  

  uint32_t period = rx_buffer[OA_SET_PWM_PERIOD_POS];
  period += (rx_buffer[OA_SET_PWM_PERIOD_POS + 1] << 8);
  period += (rx_buffer[OA_SET_PWM_PERIOD_POS + 2] << 16);
  period += (rx_buffer[OA_SET_PWM_PERIOD_POS + 3] << 24);

  uint32_t pulse =  rx_buffer[OA_SET_PWM_PULSE_POS];
  pulse += (rx_buffer[OA_SET_PWM_PULSE_POS + 1] << 8);
  pulse += (rx_buffer[OA_SET_PWM_PULSE_POS + 2] << 16);
  pulse += (rx_buffer[OA_SET_PWM_PULSE_POS + 3] << 24);


  if (checkSetMsgReceived(rx_buffer, ARG_OA_SET_PWM, LEN_OA_SET_PWM)) {
    if(ch >= OA_PWM_CHANNELS_NUM) { return true; }
    /* setting present PWM values */
    configurePwmPeriod(ch, period);
    configurePwmPulse(ch, pulse);
    prepareSetAns(tx_buffer, ANS_ARG_OA_ACK, ANS_LEN_OA_ACK);
    rv = true;
  }
  else if(checkSetMsgReceived(rx_buffer, ARD_OA_SET_DEFAULT_PWM, LEN_OA_SET_PWM)) {
    if(ch >= OA_PWM_CHANNELS_NUM) { return true; }
    pwm_period_defaults[ch] = period;
    pwm_pulse_defaults[ch] = pulse;
    return true;
  }

  if(rv) {
    prepareSetAns(tx_buffer, ANS_ARG_OA_ACK, ANS_LEN_OA_ACK);
  }
  return rv;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool OptaAnalog::parse_get_di_value() {
  if (checkGetMsgReceived(rx_buffer, ARG_OA_GET_DI,LEN_OA_GET_DI)) {
    tx_buffer[ANS_OA_GET_DI_VALUE_POS] = digital_ins;
    prepareGetAns(tx_buffer, ANS_ARG_OA_GET_DI,ANS_LEN_OA_GET_DI);
    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool OptaAnalog::parse_set_led() {
  if (checkSetMsgReceived(rx_buffer, ARG_OA_SET_LED,LEN_OA_SET_LED)) {
    led_status = rx_buffer[OA_SET_LED_VALUE_POS];
    prepareSetAns(tx_buffer, ANS_ARG_OA_ACK,ANS_LEN_OA_ACK);
    return true;
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool OptaAnalog::parse_get_channel_func() {
   if (checkGetMsgReceived(rx_buffer, ARG_GET_CHANNEL_FUNCTION, LEN_GET_CHANNEL_FUNCTION)) {
    uint8_t ch = rx_buffer[GET_CHANNEL_FUNCTION_CH_POS];
    if(ch < OA_AN_CHANNELS_NUM) {
      tx_buffer[ANS_GET_CHANNEL_FUNCTION_CH_POS] = ch;
      tx_buffer[ANS_GET_CHANNEL_FUNCTION_FUN_POS] = output_fun[ch];
      if(rtd[ch].use_3_wires && rtd[ch].is_rtd) {
        tx_buffer[ANS_GET_CHANNEL_FUNCTION_FUN_POS] = CH_FUNC_RESISTANCE_MEASUREMENT_3_WIRES;
      }
      prepareGetAns(tx_buffer, ANS_GET_CHANNEL_FUNCTION, LEN_ANS_GET_CHANNEL_FUNCTION);

    }
    return true;
  }
  return false;

}


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

int OptaAnalog::parse_rx() {
  timer_call_num = 0;
  _resetOutputs(false);
  /* call base version because here are handled assign
     addresses messages NOTE: this must be done for
     every other expansion type derived from Module */

  int rv = Module::parse_rx();

  if (rv != -1) {
    /* no need for other msg parsing (message parsed in
     * base version) */
    return rv;
  }

  rv = -1;

  /* Parse OPTA ANALOG I2C messages */
#if defined DEBUG_SERIAL && defined DEBUG_ANALOG_PARSE_MESSAGE
  Serial.print("*** ANALOG PARSING MESSAGE: ");
#endif

  if (parse_get_adc_value()) {
#if defined DEBUG_SERIAL && defined DEBUG_ANALOG_PARSE_MESSAGE
    Serial.println("get   ADC channel");
#endif
    rv = getExpectedAnsLen(ANS_LEN_OA_GET_ADC);
  } else if (parse_get_all_adc_value()) {
#if defined DEBUG_SERIAL && defined DEBUG_ANALOG_PARSE_MESSAGE
    Serial.println("get   ADC channel (ALL version)");
#endif
    rv = getExpectedAnsLen(ANS_LEN_OA_GET_ALL_ADC);
  } else if (parse_get_rtd_value()) {
#if defined DEBUG_SERIAL && defined DEBUG_ANALOG_PARSE_MESSAGE
    Serial.println("get   RTD value");
#endif
    rv = getExpectedAnsLen(ANS_LEN_OA_GET_RTD);
  } else if (parse_get_di_value()) {
#if defined DEBUG_SERIAL && defined DEBUG_ANALOG_PARSE_MESSAGE
    Serial.println("get   DI value");
#endif
    rv = getExpectedAnsLen(ANS_LEN_OA_GET_DI);
  } else if (parse_set_led()) {
#if defined DEBUG_SERIAL && defined DEBUG_ANALOG_PARSE_MESSAGE
    Serial.println("set   LED status");
#endif
    rv = getExpectedAnsLen(ANS_LEN_OA_ACK);
  } else if (parse_setup_adc_channel()) {
#if defined DEBUG_SERIAL && defined DEBUG_ANALOG_PARSE_MESSAGE
    Serial.println("begin ADC channel");
#endif
    rv = getExpectedAnsLen(ANS_LEN_OA_ACK);
  } else if (parse_set_dac_value()) {
#if defined DEBUG_SERIAL && defined DEBUG_ANALOG_PARSE_MESSAGE
    Serial.println("set   DAC value");
#endif
    rv = getExpectedAnsLen(ANS_LEN_OA_ACK);
  } else if (parse_set_rtd_update_rate()) {
#if defined DEBUG_SERIAL && defined DEBUG_ANALOG_PARSE_MESSAGE
    Serial.println("begin RTD CURRENT");
#endif
    rv = getExpectedAnsLen(ANS_LEN_OA_ACK);
  } else if (parse_set_pwm_value()) {
#if defined DEBUG_SERIAL && defined DEBUG_ANALOG_PARSE_MESSAGE
    Serial.println("set PWM value");
#endif
    rv = getExpectedAnsLen(ANS_LEN_OA_ACK);
  } else if (parse_setup_rtd_channel()) {
#if defined DEBUG_SERIAL && defined DEBUG_ANALOG_PARSE_MESSAGE
    Serial.println("begin RTD channel");
#endif
    rv = getExpectedAnsLen(ANS_LEN_OA_ACK);
  } else if (parse_setup_dac_channel()) {
#if defined DEBUG_SERIAL && defined DEBUG_ANALOG_PARSE_MESSAGE
    Serial.println("begin DAC channel");
#endif
    rv = getExpectedAnsLen(ANS_LEN_OA_ACK);
  } else if (parse_setup_di_channel()) {
#if defined DEBUG_SERIAL && defined DEBUG_ANALOG_PARSE_MESSAGE
    Serial.println("begin DI channel");
#endif
    rv = getExpectedAnsLen(ANS_LEN_OA_ACK);
  } else if (parse_set_all_dac_value()) {
#if defined DEBUG_SERIAL && defined DEBUG_ANALOG_PARSE_MESSAGE
    Serial.println("set all DAC values");
#endif
    rv = getExpectedAnsLen(ANS_LEN_OA_ACK);
  } else if (parse_setup_high_imp_channel()) {
#if defined DEBUG_SERIAL && defined DEBUG_ANALOG_PARSE_MESSAGE
    Serial.println("begin HIGH IMPEDENCE channel");
#endif
    rv = getExpectedAnsLen(ANS_LEN_OA_ACK);
  } else if(parse_get_channel_func()) {
#if defined DEBUG_SERIAL && defined DEBUG_ANALOG_PARSE_MESSAGE
    Serial.println("get channel function");
#endif       
    rv = getExpectedAnsLen(LEN_ANS_GET_CHANNEL_FUNCTION);
  } 
  else {
#if defined DEBUG_SERIAL && defined DEBUG_ANALOG_PARSE_MESSAGE    
    Serial.println(" !!! MESSAGE UNKNOWN !!!");
#endif    
  }
  return rv;
}


bool OptaAnalog::are_all_dac_updated() {
  bool rv = true;
  for(int ch = 0; ch < OA_AN_CHANNELS_NUM; ch++) {
    if(!dac_value_updated[ch]) {
      rv = false;
      break;
    }
  }
  return rv;
}

bool OptaAnalog::configuration_to_be_updated() {
  bool rv = false;
  for(int ch = 0; ch < OA_AN_CHANNELS_NUM; ch++) {
    if(update_fun[ch].size() > 0) {
      rv = true;
      break;
    }
  }
  return rv;
}


void OptaAnalog::setup_channels() {
  if (configuration_to_be_updated()) {
    stopAdc();
    ChConfig cfg;
    for (uint8_t ch = 0; ch < OA_AN_CHANNELS_NUM; ch++) {
      if (update_fun[ch].size() > 0) {
        /* new function has been requested */
        cfg = update_fun[ch].front();
        update_fun[ch].pop();
        
        /* disable adc conversion (it will be enable in any case after) */
        if (adc[ch].en_conversion) {
          configureAdcEnable(ch, false);
        }

        if(cfg.write) {
          /* channel must be changed only when an actual request of new function
             is active and not when an ADC is added to a channel */

          /* if the current function is DAC or RTD reset output value 
             put the switch in the correct position for channel 0 or 1
             remember that 3 wires RTD is actually a DAC current ! */
          if (fun[ch] == CH_FUNC_VOLTAGE_OUTPUT ||
              fun[ch] == CH_FUNC_CURRENT_OUTPUT || 
              fun[ch] == CH_FUNC_RESISTANCE_MEASUREMENT) { 

            reset_dac_value(ch);

            if (ch == 0) {
              digitalWrite(DIO_RTD_SWITCH_1, LOW);
            }
            if (ch == 1) {
              digitalWrite(DIO_RTD_SWITCH_2, LOW);
            }
          }

          /* if the current function is not HIGH IMPEDENCE go in HI 
            (this is necessary to change the function on a channel) */
          if (fun[ch] != CH_FUNC_HIGH_IMPEDENCE) {
            write_reg(OA_REG_FUNC_SETUP, CH_HIGH_IMP, ch);
            delay(5);
          }

          /* write the new function only if the new f*/
        
          if (cfg.f != CH_FUNC_HIGH_IMPEDENCE) {
            sendFunction(ch, cfg.f);
          }

          /* output_fun cannot be equal to fun */
          output_fun[ch] = cfg.f;

        } //if(write_function_configuration[ch]) {
           /* update the function */
        fun[ch] = cfg.f;
      }

      /* update the channel configurazione (to be done always also in case
         of add an ADC channel */

      switch (cfg.f) {
      case CH_FUNC_HIGH_IMPEDENCE:
        break;
      case CH_FUNC_VOLTAGE_OUTPUT:
        sendDacConfiguration(ch);
        update_dac_using_LDAC = true;
        break;
      case CH_FUNC_CURRENT_OUTPUT:
        sendDacConfiguration(ch);
        update_dac_using_LDAC = true;
        break;
      case CH_FUNC_VOLTAGE_INPUT:
        configureAdcEnable(ch, true);
        sendAdcConfiguration(ch);
        break;
      case CH_FUNC_CURRENT_INPUT_EXT_POWER:
        configureAdcEnable(ch, true);
        sendAdcConfiguration(ch);
        break;
      case CH_FUNC_CURRENT_INPUT_LOOP_POWER:
        configureAdcEnable(ch, true);
        sendAdcConfiguration(ch);
        break;
      case CH_FUNC_RESISTANCE_MEASUREMENT:
        configureAdcEnable(ch, true);
        break;
      case CH_FUNC_DIGITAL_INPUT:
        sendDinConfiguration(ch);
        break;
      case CH_FUNC_DIGITAL_INPUT_LOOP_POWER:
        sendDinConfiguration(ch);
        break;
      default:
        break;
      }
    }
    startAdc();
  }
}

uint8_t OptaAnalog::getMajorFw() { return FW_VERSION_MAJOR; }
uint8_t OptaAnalog::getMinorFw() { return FW_VERSION_MINOR; }
uint8_t OptaAnalog::getReleaseFw() { return FW_VERSION_RELEASE; }

std::string OptaAnalog::getProduct() {
  std::string rv(OPTA_ANALOG_DESCRIPTION);
  return rv;
}

/* -------------------------------------------------------------------------- */
void OptaAnalog::timer_callback(timer_callback_args_t *arg) {
  /* ------------------------------------------------------------------------ */
  OptaAnalog *ptr = nullptr;
  if (arg != nullptr) {
    ptr = (OptaAnalog *)arg->p_context;
  }

  if (ptr != nullptr) {
    ptr->_incrementTimerCallNum();

    if (ptr->_timerElapsed()) {
      ptr->_resetTimerCallNum();
      if (ptr->_timeIsNotForever()) {
        ptr->_resetOutputs(true);
      }
    }
  }
}


void OptaAnalog::set_up_timer() {
  
  uint8_t type;
  int8_t num = FspTimer::get_available_timer(type);
  if (num >= 0) {
    timer.begin(TIMER_MODE_PERIODIC, type, num, 1000, 50, timer_callback, this);
    timer.setup_overflow_irq(14);
    timer.open();
    timer.start();
  }
}


void OptaAnalog::_resetOutputs(bool flag) {
  use_default_output_values = flag;
}


void OptaAnalog::goInBootloaderMode() { goBootloader(); }

void OptaAnalog::writeInFlash(uint16_t add, uint8_t *buffer, uint8_t dim) {
  if (dim > 32) {
    dim = 32;
  }
  uint8_t data[32];
  memset((uint8_t *)data, 0, dim);
  for (int i = 0; i < dim; i++) {
    data[i] = *(buffer + i);
  }
  EEPROM.put(add, data);
}

void OptaAnalog::readFromFlash(uint16_t add, uint8_t *buffer, uint8_t dim) {
  if (dim > 32) {
    dim = 32;
  }
  uint8_t data[32];
  memset((uint8_t *)data, 0, dim);
  EEPROM.get(add, data);
  for (int i = 0; i < dim; i++) {
    *(buffer + i) = *(data + i);
  }
}
void OptaAnalog::initStatusLED() {
  pinMode(OPTA_LED_RED, OUTPUT);
  pinMode(OPTA_LED_BLUE, OUTPUT);
  pinMode(OPTA_LED_GREEN, OUTPUT);
  digitalWrite(OPTA_LED_RED, LED_RGB_OFF);
  digitalWrite(OPTA_LED_BLUE, LED_RGB_OFF);
  digitalWrite(OPTA_LED_GREEN, LED_RGB_OFF);
}

void OptaAnalog::setStatusLedReadyForAddress() {
  digitalWrite(OPTA_LED_BLUE, LED_RGB_OFF);
  digitalWrite(OPTA_LED_RED, LED_RGB_ON);
  digitalWrite(OPTA_LED_GREEN, LED_RGB_OFF);
}

void OptaAnalog::setStatusLedWaitingForAddress() {
  digitalWrite(OPTA_LED_BLUE, LED_RGB_ON);
  digitalWrite(OPTA_LED_RED, LED_RGB_OFF);
  digitalWrite(OPTA_LED_GREEN, LED_RGB_OFF);
}

void OptaAnalog::setStatusLedHasAddress() {
  digitalWrite(OPTA_LED_BLUE, LED_RGB_OFF);
  digitalWrite(OPTA_LED_RED, LED_RGB_OFF);
  digitalWrite(OPTA_LED_GREEN, LED_RGB_ON);
}
#ifdef DEBUG_SERIAL
void print_function(uint8_t v) {
  switch (v) {
  case 0:
    Serial.print("|    HIGH IMPEDENCE  |");
    break;
  case 1:
    Serial.print("|     DAC VOLTAGE    |");
    break;
  case 2:
    Serial.print("|     DAC CURRENT    |");
    break;
  case 3:
    Serial.print("|     ADC VOLTAGE    |");
    break;
  case 4:
    Serial.print("|  ADC CURRENT (ep)  |");
    break;
  case 5:
    Serial.print("|  ADC CURRENT (lp)  |");
    break;
  case 6:
    Serial.print("|     RESISTANCE     |");
    break;
  case 7:
    Serial.print("|    DIGITAL INPUT   |");
    break;
  case 8:
    Serial.print("| DIGITAL INPUT (lp) |");

    break;
  }
  Serial.print(" ");
}

void print_adc_configuration(uint16_t v) {
  Serial.print("ADC: ");
  if (v & 1) {
    Serial.print("cur ");
  } else {
    Serial.print("vol ");
  }

  if (v & 4) {
    Serial.print("pull_Y ");
  } else {
    Serial.print("pull_N ");
  }

  if (v & 8) {
    Serial.print("rej_Y ");
  } else {
    Serial.print("rej_N ");
  }

  uint8_t m = (v & (7 << 5)) >> 5;

  if (m == 0) {
    Serial.print("10.0V ");
  } else if (m == 1) {
    Serial.print("2.5V RTD ");
  } else if (m == 2) {
    Serial.print("2.5V LOOP ");
  } else if (m == 3) {
    Serial.print("2.5V (3) ");
  }
}

void print_adc_control(uint16_t v, int d) {
  int ch = (d == OA_AN_DEVICE_0) ? 1 : 2;
  Serial.print(" ");
  Serial.print("# ADC CONTROL: ");
  if (v & 1) {
    Serial.print(String(ch) + " YES ");
  } else {
    Serial.print(String(ch) + " NO ");
  }
  ch = (d == OA_AN_DEVICE_0) ? 0 : 3;
  if (v & 2) {
    Serial.print(String(ch) + " YES ");
  } else {
    Serial.print(String(ch) + " NO ");
  }
  ch = (d == OA_AN_DEVICE_0) ? 6 : 4;
  if (v & 4) {
    Serial.print(String(ch) + " YES ");
  } else {
    Serial.print(String(ch) + " NO ");
  }
  ch = (d == OA_AN_DEVICE_0) ? 7 : 5;
  if (v & 8) {
    Serial.print(String(ch) + " YES ");
  } else {
    Serial.print(String(ch) + " NO ");
  }
  ch = (d == OA_AN_DEVICE_0) ? 1 : 2;
  if (v & 16) {
    // Serial.println("- Ch " + String(ch) + " ADC diag enabled");
  } else {
    // Serial.println("- Ch " + String(ch) + " ADC diag DISABLED");
  }
  ch = (d == OA_AN_DEVICE_0) ? 0 : 3;
  if (v & 32) {
    // Serial.println("- Ch " + String(ch) + " ADC diag enabled");
  } else {
    // Serial.println("- Ch " + String(ch) + " ADC diag DISABLED");
  }
  ch = (d == OA_AN_DEVICE_0) ? 6 : 4;
  if (v & 64) {
    // Serial.println("- Ch " + String(ch) + " ADC diag enabled");
  } else {
    // Serial.println("- Ch " + String(ch) + " ADC diag DISABLED");
  }
  ch = (d == OA_AN_DEVICE_0) ? 7 : 5;
  if (v & 128) {
    // Serial.println("- Ch " + String(ch) + " ADC diag enabled");
  } else {
    // Serial.println("- Ch " + String(ch) + " ADC diag DISABLED");
  }

  if (v & (1 << 10)) {
    // Serial.println("- DIAGNOSTIC REJECTION enabled");
  } else {
    // Serial.println("- DIAGNOSTIC REJECTION DISABLED");
  }

  uint8_t m = (v & (3 << 8)) >> 8;

  if (m == 0) {
    Serial.print(" STOPPED AND POWERED UP ");
  } else if (m == 1) {
    Serial.print(" STARTED SINGLE CONVERSION ");
  } else if (m == 2) {
    Serial.print(" STARTED CONTINUOS ");
  } else if (m == 3) {
    Serial.print(" STOPPED AND POWERED DOWN ");
  }
}

void print_output_configuration(uint16_t v) {
  Serial.print("DAC: ");
  if (v & 1) {
    Serial.print("7.5mA ");
  } else {
    Serial.print("29mA ");
  }
  if (v & 2) {
    Serial.print("CL_Y ");
  } else {
    Serial.print("CL_N ");
  }
  if ((v & (1 << 6)) && (v & (1 << 7) == 0)) {
    Serial.print("SL_Y ");
  } else {
    Serial.print("SL_N ");
  }
  uint8_t m = (v & (3 << 2)) >> 2;

  if (m == 0) {
    Serial.print("4 kHz ");
  } else if (m == 1) {
    Serial.print("64 kHz ");
  } else if (m == 2) {
    Serial.print("150 kHz ");
  } else if (m == 3) {
    Serial.print("240 kHz ");
  }
  m = (v & (3 << 4)) >> 4;

  if (m == 0) {
    Serial.print("64 bit ");
  } else if (m == 1) {
    Serial.print("120 bit ");
  } else if (m == 2) {
    Serial.print("500 bit ");
  } else if (m == 3) {
    Serial.print("1820 bit ");
  }
}

void print_digital_input_configuration(uint16_t v1, uint16_t v2) {
  uint8_t dt = v1 & 0x1F;
  Serial.print("DI: ");
  switch (dt) {
  case 0:
    Serial.print("time NONE ");
    break;
  case 1:
    Serial.print("time 13us ");
    break;
  case 2:
    Serial.print("time 18.7us ");
    break;
  case 3:
    Serial.print("time 24.4us ");
    break;
  case 4:
    Serial.print("time 32.5us ");
    break;
  case 5:
    Serial.print("time 42.3us ");
    break;
  case 6:
    Serial.print("time 56.1us ");
    break;
  case 7:
    Serial.print("time 75.6us ");
    break;
  case 8:
    Serial.print("time 100.8us ");
    break;
  case 9:
    Serial.print("time 130.1us ");
    break;
  case 10:
    Serial.print("time 180.5us ");
    break;
  case 11:
    Serial.print("time 240.6us ");
    break;
  case 12:
    Serial.print("time 320.3us ");
    break;
  case 13:
    Serial.print("time 420.3us ");
    break;
  case 14:
    Serial.print("time 560.2us ");
    break;
  case 15:
    Serial.print("time 750.4us ");
    break;
  case 16:
    Serial.print("time 1ms ");
    break;
  case 17:
    Serial.print("time 1.8ms ");
    break;
  case 18:
    Serial.print("time 2.4ms ");
    break;
  case 19:
    Serial.print("time 3.2ms ");
    break;
  case 20:
    Serial.print("time 4.2ms ");
    break;
  case 21:
    Serial.print("time 5.6ms ");
    break;
  case 22:
    Serial.print("time 7.5ms ");
    break;
  case 23:
    Serial.print("time 10ms ");
    break;
  case 24:
    Serial.print("time 13ms ");
    break;
  case 25:
    Serial.print("time 18ms ");
    break;
  case 26:
    Serial.print("time 18ms ");
    break;
  case 27:
    Serial.print("time 24ms ");
    break;
  case 28:
    Serial.print("time 32ms ");
    break;
  case 29:
    Serial.print("time 42ms ");
    break;
  case 30:
    Serial.print("time 56ms ");
    break;
  case 31:
    Serial.print("time 75ms ");
    break;
  }
  if (v1 & 32) {
    Serial.print("INTEGRATOR ");
  } else {
    Serial.print("SIMPLE ");
  }

  uint8_t isink = (v1 & (0xF << 6)) >> 6;
  Serial.print(" sink ");
  Serial.print(isink * 120);
  Serial.print("uA ");
  if (v1 & 0x1000) {
    Serial.print("comparator ENABLED ");
  } else {
    Serial.print("comparator DISABLED ");
  }
  if (v1 & 0x2000) {
    Serial.print("output INVERTED ");
  } else {
    Serial.print("output not inverted ");
  }
  if (v1 & 0x4000) {
    Serial.print("FILTERED ");
  } else {
    Serial.print("not FILTERED ");
  }
  if (v2 & 1) {
    Serial.print("th 16V");
  } else {
    Serial.print("th VDD");
  }
  Serial.println();
}

void OptaAnalog::debugDacFunction(uint8_t ch) {
  uint16_t reg = 0;
  read_reg(0x12, reg, ch);
  print_output_configuration(reg);
}

int OptaAnalog::debugChannelFunction(uint8_t ch) {
  Serial.print("# CHANNEL ");
  Serial.print(ch);
  Serial.print(" ");
  uint16_t reg = 0;
  if (!read_reg(0x01, reg, ch)) {
    Serial.println("ERROR while reading");
  }
  print_function((reg & 0xF));
  return (reg & 0xF);
}
void OptaAnalog::debugAdcConfiguration(uint8_t ch) {
  uint16_t reg = 0;
  read_reg(0x05, reg, ch);
  print_adc_configuration(reg);
}
void OptaAnalog::debugDiConfiguration(uint8_t ch) {
  uint16_t r2 = 0;
  uint16_t reg = 0;
  read_reg(0x09, reg, ch);
  uint8_t device = GET_DEVICE_FROM_CHANNEL(ch);

  read_direct_reg(device, 0x22, r2);
  print_digital_input_configuration(reg, r2);
}

void OptaAnalog::displayOaDebugInformation() {
  /* This function is for debug only and list all the
   * settings for the analog device */
  uint16_t reg = 0;
  Serial.println("\n**********************************"
                 "****************");
  Serial.println("*               DEVICE "
                 "configuration             *");
  Serial.println("************************************"
                 "**************\n");
  for (int i = 0; i < OA_AN_CHANNELS_NUM; i++) {
    int ch_function = debugChannelFunction(i);
    debugAdcConfiguration(i);
    debugDacFunction(i);
    debugDiConfiguration(i);
  }
#ifdef ARDUINO_OPTA_ANALOG
  read_direct_reg(OA_AN_DEVICE_0, 0x23, reg);
  
  print_adc_control(reg, OA_AN_DEVICE_0);
  read_direct_reg(OA_AN_DEVICE_1, 0x23, reg);
  
  print_adc_control(reg, OA_AN_DEVICE_1);
  Serial.println();
#else
  read_direct_reg(OA_AN_DEVICE_0, 0x23, reg);
  print_adc_control(reg, OA_AN_DEVICE_0);
#endif
}

void printFunction(CfgFun_t f) {
  if(f == CH_FUNC_HIGH_IMPEDENCE) {
    Serial.print("High Impedence");
  } else if(f == CH_FUNC_VOLTAGE_OUTPUT) {
    Serial.print("DAC Voltage");
  }
  else if(f == CH_FUNC_CURRENT_OUTPUT) {
    Serial.print("DAC Current");
  }
  else if(f == CH_FUNC_VOLTAGE_INPUT) {
    Serial.print("ADC Voltage");
  }
  else if(f == CH_FUNC_CURRENT_INPUT_EXT_POWER) {
    Serial.print("ADC Current");
  }
  else if(f == CH_FUNC_CURRENT_INPUT_LOOP_POWER) {
    Serial.print("ADC Current loop");
  }
  else if(f == CH_FUNC_RESISTANCE_MEASUREMENT) {
    Serial.print("RTD");
  }
  else if(f == CH_FUNC_DIGITAL_INPUT) {
    Serial.print("Digital Input");
  }
  else if(f == CH_FUNC_DIGITAL_INPUT_LOOP_POWER) {
    Serial.print("Digital Input loop");
  }
  else if(f == CH_FUNC_UNDEFINED) {
    Serial.print("Undefined");
  }
  else {
    Serial.print("BOH?");
  }
}


void OptaAnalog::debugPrintChannelFunction(uint8_t ch) {
  Serial.print("Ch " + String(ch) + " Fun ");
  printFunction(fun[ch]);
  Serial.println();

}

void OptaAnalog::debugPrintDac() {
  Serial.println();
  for(int ch = 0; ch < 8; ch++) {
    Serial.println("CHANNEL " + String(ch));
    uint16_t hw_function_channel = 0;
    read_reg(0x01, hw_function_channel, ch);
    hw_function_channel = hw_function_channel & 0x0F;
    if(hw_function_channel == 1) {
      Serial.println("VOLTAGE DAC");
    }
    else if(hw_function_channel == 2) {
      Serial.println("CURRENT DAC");
    }
    else {
      Serial.println("IS NOT DAC " + String(hw_function_channel) );
    }

    if(hw_function_channel == 1 || hw_function_channel == 2) {
      uint16_t dac_user_set_value = dac_values[ch];
      uint16_t dac_register_setting = 0;
      read_reg(0x16, dac_register_setting, ch);
      uint16_t dac_active_setting = 0;
      read_reg(0x1E, dac_active_setting, ch);
      Serial.println("User setting: " +  String(dac_user_set_value) + " output setting " + String(dac_register_setting) + " active setting " + String(dac_active_setting));
    }

  }

}
#endif
#endif
#endif
