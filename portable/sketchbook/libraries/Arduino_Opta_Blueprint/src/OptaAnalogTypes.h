/* -------------------------------------------------------------------------- */
/*
   FILE NAME:   OptaAnalogTypes.h
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        31102023
   DESCRIPTION: Contains definition of types used to handle Opta Digital HW
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:
*/
/* -------------------------------------------------------------------------- */

#ifndef OPTA_ANALOG_TYPES_BLUE_MODULE
#define OPTA_ANALOG_TYPES_BLUE_MODULE

#include <cstdint>
#if defined ARDUINO_OPTA_ANALOG || defined ARDUINO_UNO_TESTALOG_SHIELD
#include "OptaAnalogCfg.h"
#include "pwm.h"
#include "sys/_stdint.h"
#include <stdint.h>


/* ########################## ADC configuration ############################# */

typedef enum {
  CFG_ADC_INPUT_NODE_IOP_AGND_SENSE,
  CFG_ADC_INPUT_NODE_100OHM_R
} CfgAdcMux_t;

typedef enum {
  CFG_ADC_RANGE_10V,
  CFG_ADC_RANGE_2_5V_RTD,
  CFG_ADC_RANGE_2_5V_LOOP,
  CFG_ADC_RANGE_2_5V_BI
} CfgAdcRange_t;

class CfgAdc {
public:
  CfgAdcMux_t mux;
  CfgAdcRange_t range;
  bool en_rejection;
  bool en_pull_down;
  bool en_conversion_diagnostic;
  bool en_conversion;
  uint16_t conversion;
  uint16_t diag_conversion;

  double average;
  uint8_t average_samples;

  /* */
  uint8_t mov_average_req;
  /* default ADC configuration */
  CfgAdc()
      : mux(CFG_ADC_INPUT_NODE_IOP_AGND_SENSE), range(CFG_ADC_RANGE_10V),
        en_rejection(true), en_pull_down(false),
        en_conversion_diagnostic(false), en_conversion(false), average(0.0),
        average_samples(0), mov_average_req(0) {}
};

/* ###################### DIGITAL IN configuration ########################## */
class CfgDi {
public:
  bool filter_input_comparator;
  bool invert_comparator_output;
  bool enable_comparator;
  bool debounce_mode_simple; // false means integrator

  uint8_t current_sink;
  uint8_t debounce_time;
  CfgDi()
      : filter_input_comparator(true), invert_comparator_output(false),
        enable_comparator(true), debounce_mode_simple(true), current_sink(0),
        debounce_time(0xB) {}
};

/* ########################## GPO configuration ############################# */

class CfgPwm {
public:
  PwmOut pwm;
  /* period_us is the period set to the pwm (it is updated only when different
   * from set_period_us, this is necessary because if the period is continuously
   * updated the timer does not work as expected and the is not set properly)*/
  uint32_t period_us;
  /* same as per period_us */
  uint32_t pulse_us;
  /* set_period_us is the period set point */
  uint32_t set_period_us;
  /* set_pulse_us is the pulse set point */
  uint32_t set_pulse_us;
  CfgPwm(int p)
      : pwm(p), period_us(0), pulse_us(0), set_period_us(0),
        set_pulse_us(0) {}
  void begin() {
    pwm.begin();
    pwm.suspend();
  }
};

/* ########################## GPO configuration ############################# */

typedef enum { GPO_LOW, GPO_HIGH } GpoState_t;

typedef enum {
  GPO_PULL_DOWN,
  GPO_DRIVEN_SINGLE,
  GPO_DRIVEN_PARALLEL,
  GPO_OUT_DEB_COMP,
  GPO_HIGH_IMP
} CfgGpo_t;

class CfgGpo {
public:
  CfgGpo_t cfg;
  GpoState_t state;
  CfgGpo() {
    cfg = GPO_HIGH_IMP;
    state = GPO_LOW;
  }
};

/* ########################## DAC configuration ############################# */

typedef enum {
  OUT_CURRENT_LIMIT_30mA,
  OUT_CURRENT_LIMIT_7_5mA
} CfgOutCurrLim_t;

typedef enum {
  OUT_SLEW_RATE_4k,
  OUT_SLEW_RATE_64k,
  OUT_SLEW_RATE_150k,
  OUT_SLEW_RATE_240k
} CfgOutSlewRate_t;

typedef enum {
  OUT_SLEW_STEP_64,
  OUT_SLEW_STEP_120,
  OUT_SLEW_STEP_500,
  OUT_SLEW_STEP_1820
} CfgOutSlewStep_t;

class CfgDac {
public:
  CfgOutCurrLim_t current_limit;
  CfgOutSlewRate_t slew_rate;
  CfgOutSlewStep_t slew_step;
  uint16_t reset_value;   // value when a CLEAR_DAC is issued
  uint16_t present_value; // value the DAC is using due to slew rate
                          // limitation set
  bool enable_clear;
  bool enable_slew;
  CfgDac() {
    reset_value = 0;
    current_limit = OUT_CURRENT_LIMIT_7_5mA;
    slew_rate = OUT_SLEW_RATE_4k;
    slew_step = OUT_SLEW_STEP_64;
    enable_clear = false;
    enable_slew = false;
  }
};
class CfgRtd {
public:
  bool is_rtd;
  bool use_3_wires;
  double v_RTD_2RL;
  double v_RTD_RL;
  double RRTD_2RL;
  double RRTD_RL;
  double RTD;
  uint16_t current_value;
  double i_excite;

  void set_measure_current(float current) {
    // DAC can supply 25 when 8191 is set
    float v = current * 8191;
    v /= 25.0;
    current_value = (uint16_t)v;
  }
  void set_adc_RTD_2RL(uint16_t adc_value) {
#if defined DEBUG_SERIAL && defined DEBUG_RTD
    Serial.print("   RTD+2RL adc_value: ");
    Serial.print(adc_value);
#endif
    v_RTD_2RL = (((double)(adc_value)) / 65535.0);
    v_RTD_2RL *= 2.5;
    v_RTD_2RL -= 0.0; // TO BE VERIFIED
#if defined DEBUG_SERIAL && defined DEBUG_RTD
    Serial.print(" v_RTD_2RL: ");
    Serial.println(v_RTD_2RL, 10);
#endif
  }
  void set_adc_RTD_RL(uint16_t adc_value) {
#if defined DEBUG_SERIAL && defined DEBUG_RTD
    Serial.print("   RTD+RL adc_value: ");
    Serial.print(adc_value);
#endif
    v_RTD_RL = (((double)adc_value) / 65535.0);
    v_RTD_RL *= 2.5;
    v_RTD_RL -= 0.0; // TO BE VERIFIED
#if defined DEBUG_SERIAL && defined DEBUG_RTD
    Serial.print(" v_RTD_RL: ");
    Serial.println(v_RTD_RL, 10);
#endif
  }
  void set_i_excite(uint16_t adc_value) {
#if defined DEBUG_SERIAL && defined DEBUG_RTD
    Serial.print("   CURRENT adc_value: ");
    Serial.print(adc_value);
#endif
    double v = (double)adc_value;
    i_excite = v * 2.5;
    i_excite /= 65535.0;
    i_excite /= 100;
#if defined DEBUG_SERIAL && defined DEBUG_RTD
    Serial.print(" i_excite: ");
    Serial.println(i_excite, 10);
#endif
  }
  void calc_RTD() {
#if defined DEBUG_SERIAL && defined DEBUG_RTD
    Serial.print(" v_RTD_2RL: ");
    Serial.println(v_RTD_2RL, 10);
    Serial.print(" i_excite: ");
    Serial.println(i_excite, 10);
    Serial.print(" v_RTD_RL: ");
    Serial.println(v_RTD_RL, 10);
#endif

    RRTD_2RL = v_RTD_2RL / i_excite;

#if defined DEBUG_SERIAL && defined DEBUG_RTD
    Serial.print(" RRTD_2RL: ");
    Serial.println(RRTD_2RL, 10);
#endif

    RRTD_RL = v_RTD_RL / i_excite;

#if defined DEBUG_SERIAL && defined DEBUG_RTD
    Serial.print(" RRTD_RL: ");
    Serial.println(RRTD_RL, 10);
#endif

    double RL = RRTD_2RL - RRTD_RL;

#if defined DEBUG_SERIAL && defined DEBUG_RTD
    Serial.print(" RL: ");
    Serial.println(RL, 10);
#endif
    RTD = RRTD_RL - RL;
#if defined DEBUG_SERIAL && defined DEBUG_RTD
    Serial.print("RDT: ");
    Serial.println(RTD);
#endif
  }
  void set(uint16_t adc_value) {
#if defined DEBUG_SERIAL && defined DEBUG_RTD
    Serial.print(" RESISTANCE adc_value: ");
    Serial.println(adc_value);
#endif
    double adc32 = (double)adc_value;
    if (65535.0 - adc32 == 0.0) {
      RTD = 0.0;
    } else {
      RTD = (adc32 * 2100.0) / (65535.0 - adc32);
    }
#if defined DEBUG_SERIAL && defined DEBUG_RTD
    Serial.print(" RESISTANCE: ");
    Serial.println(RTD, 10);
#endif
  }
  CfgRtd() : is_rtd(false), use_3_wires(false), RTD(0.0), current_value(1.2) {}
};
#endif
#endif
