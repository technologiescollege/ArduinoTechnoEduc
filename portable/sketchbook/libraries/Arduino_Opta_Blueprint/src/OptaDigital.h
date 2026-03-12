/* -------------------------------------------------------------------------- */
/* FILENAME:    OptaDigital.h
   AUTHOR:      Daniele Aimo (d.aimo@arduino.cc)
   DATE:        20230810
   REVISION:    0.0.1
   DESCRIPTION: Header file for OptaDigital class
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#ifndef OPTA_DIGITAL_BLUE_MODULE
#define OPTA_DIGITAL_BLUE_MODULE

#include "DigitalCommonCfg.h"
#include "EEPROM.h"
#include "FspTimer.h"
#include "OptaBlueModule.h"
#include "OptaDigitalCfg.h"
#include "OptaDigitalProtocol.h"
#include "OptaMsgCommon.h"
#include "analog.h"
#include "boot.h"
#include <map>
#include <stdint.h>

#define MAX_ADC_CHANNELS 29

class OptaDigitalADC : public ADC_Container {
public:
  OptaDigitalADC() = delete;
  OptaDigitalADC(void *ctx = nullptr);
};

class OptaDigital : public Module {
public:
  OptaDigital();
  virtual void end() override;
  virtual void begin() override;
  virtual void update() override;
  virtual int parse_rx() override;

  uint8_t getMajorFw();
  uint8_t getMinorFw();
  uint8_t getReleaseFw();
  std::string getProduct();
  void goInBootloaderMode();
  void readFromFlash(uint16_t add, uint8_t *buffer, uint8_t dim);
  void writeInFlash(uint16_t add, uint8_t *buffer, uint8_t dim);
  void initStatusLED();
  void setStatusLedReadyForAddress();
  void setStatusLedWaitingForAddress();
  void setStatusLedHasAddress();

  void _incrementTimerCallNum() { timer_call_num++; }
  void _resetTimerCallNum() { timer_call_num = 0; }
  bool _timeIsNotForever() { return (timer_elapsed_ms < 0xFFFF); }
  bool _timerElapsed() { return (timer_call_num >= timer_elapsed_ms); }
  unsigned int _timerCallNum() { return timer_call_num; }
  void _resetOutputs();

  /* this 2 functions are intended to be used when Opta Digital is used
   * standalone for test purposes */
  void _setDigitalOut(int n, bool value);
  bool _getDigitalOut(int n);
  void _updateDigitalOut();
  void _avoidTimeout() { timer_elapsed_ms = 0xFFFF; }

  static void adcCb(adc_callback_args_t *p_args);
  adc_ctrl_t *getAdcCtrl() { return &(opta_adc.ctrl); }

#ifdef EN_DIGITAL_STANDALONE
  uint16_t getAnalog(uint8_t p);
#endif

protected:
  virtual void reset() override;

private:
  static volatile bool conversion_performed;

  bool parse_set_digital();
  bool parse_get_digital();
  bool parse_get_analog();
  bool parse_get_all_analog();
  bool parse_default_and_timeout();

  int prepare_ans_get_digital();
  int prepare_ans_get_analog(int index);
  int prepare_ans_get_all_analog();
  int prepare_ans_default_and_timeout();
  int prepare_ans_set_digital();

  FspTimer timer;
  void set_up_timer();
  volatile unsigned int timer_call_num = 0;
  unsigned int timer_elapsed_ms = OPTA_DIGITAL_WATCHTDOG_TIME_ms;
  static void timer_callback(timer_callback_args_t *arg);

  OptaDigitalADC opta_adc;

  uint8_t ans_get_din_buffer[OPTA_DIGITAL_GET_DIN_BUFFER_DIM] = {0};
  uint8_t ans_get_all_ain_buffer[OPTA_DIGITAL_GET_ALL_AIN_BUFFER_DIM] = {0};
  uint16_t channel_analog_values[MAX_ADC_CHANNELS] = {0};
  bool digital_out[OPTA_DIGITAL_OUT_NUM] = {false};

  bool default_output[OPTA_DIGITAL_OUT_NUM] = {false};

#ifdef EN_DIGITAL_STANDALONE
  uint16_t analog_inputs[OPTA_DIGITAL_IN_NUM];
#endif

#ifdef DEBUG_UPDATE_FW
  void debug_with_leds();
#endif

  std::map<int, int> in_map;
  std::map<int, int> out_map;
};

#endif
