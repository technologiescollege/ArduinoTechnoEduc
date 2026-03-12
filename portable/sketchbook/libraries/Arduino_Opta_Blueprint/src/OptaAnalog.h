/* -------------------------------------------------------------------------- */
/* FILENAME:    OptaAnalog.h
   AUTHOR:      Daniele Aimo (d.aimo@arduino.cc)
   DATE:        20230801
   REVISION:    0.0.1
   DESCRIPTION: Header file for OptaAnalog class
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#ifndef OPTA_ANALOG_BLUE_MODULE
#define OPTA_ANALOG_BLUE_MODULE

#if defined ARDUINO_OPTA_ANALOG || defined ARDUINO_UNO_TESTALOG_SHIELD
#include "AnalogCommonCfg.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "OptaAnalogProtocol.h"
#include "OptaAnalogTypes.h"
#include "OptaBlueModule.h"
#include "OptaCrc.h"
#include "OptaMsgCommon.h"
#include "SPI.h"
#include "boot.h"
#include "sys/_stdint.h"
#include <cstdint>
#include <queue>

// #define DEBUG_ENABLE_SPI
// #define DEBUG_SERIAL
// #define DEBUG_ANALOG_PARSE_MESSAGE
//
#define SPI_COMM_BUFF_DIM 4


class ChConfig {
public:
  CfgFun_t f;
  bool write;
  ChConfig() {
    f = CH_FUNC_UNDEFINED;
    write = false;
  }
}; 



class OptaAnalog : public Module {
private:
  /* ---------------------------------------------------------------------
   * Data structures used to hold information about Analog Device AD74412R
   * --------------------------------------------------------------------- */
  CfgFun_t fun[OA_AN_CHANNELS_NUM]; // function configuration x channel
  std::queue<ChConfig> update_fun[OA_AN_CHANNELS_NUM]; // function update configuration x channel
  CfgFun_t output_fun[OA_AN_CHANNELS_NUM]; // function for output
  CfgPwm pwm[OA_PWM_CHANNELS_NUM];  // pwm configuration x channel
  CfgAdc adc[OA_AN_CHANNELS_NUM];   // adc configuration x channel
  CfgDi din[OA_AN_CHANNELS_NUM];    // d[igital]i[nput] configuration x channel
  CfgGpo gpo[OA_AN_CHANNELS_NUM];   // gpo configuration x channel
  CfgDac dac[OA_AN_CHANNELS_NUM];   // dac configuration x channel
  CfgRtd rtd[OA_AN_CHANNELS_NUM];   // rtd configuration x channel

  volatile uint16_t dac_defaults[OA_AN_CHANNELS_NUM];
  volatile uint32_t pwm_period_defaults[OA_PWM_CHANNELS_NUM];
  volatile uint32_t pwm_pulse_defaults[OA_PWM_CHANNELS_NUM];
  volatile bool dac_value_updated[OA_AN_CHANNELS_NUM];
  volatile uint16_t dac_values[OA_AN_CHANNELS_NUM];

  uint16_t live_state[OA_AN_DEVICES_NUM];
  uint16_t alert_state[OA_AN_DEVICES_NUM];

  bool en_adc_diag_rej[OA_AN_DEVICES_NUM];
  bool di_scaled[OA_AN_DEVICES_NUM];
  uint8_t di_th[OA_AN_DEVICES_NUM];
  /* the status of the digital inputs (bitmask) */
  uint8_t digital_ins;
  /* the status of the digital outputs (bitmask) */
  uint8_t gpo_digital_out;
  /* the status of the leds (bitmask) */
  uint8_t led_status;
  uint16_t rtd_update_time;
  /* ---------------------------------------------------------------------
   * SPI communication buffer and functions
   * --------------------------------------------------------------------- */
  uint8_t com_buffer[SPI_COMM_BUFF_DIM];

  uint8_t adc_ch_mask_0 = 0;
  uint8_t adc_ch_mask_1 = 0;
  uint8_t adc_ch_mask_0_last = 0;
  uint8_t adc_ch_mask_1_last = 0;

  bool update_dac_using_LDAC = false;

  void setup_channels();
  bool configuration_to_be_updated();
  bool are_all_dac_updated();

  /* ABOUT register and reading writing register with the function below
   * --------------------------------------------------------------------
   * Typically in the Analog Device AD74412R there are 2 "kind" of registers
     - channels register --> 1 register for each channel (4 register per device)
       in this case the address of the 4 registers are consecutive
     - single register --> 1 register that handle all channels

     Opta Analog uses 2 Analog Devices AD74412R at the same time. So Opta Analog
     channels are 8 from 0 to 7.

     Pay attention to the fact that the mapping of the Opta Analog channels
     (from 0 to 7) is the following:
     - ch 0 -> device 0 register channel (the displacement) 1 of AD74412R
     - ch 1 -> device 0 register channel (the displacement) 0 of AD74412R
     - ch 2 -> device 1 register channel (the displacement) 0 of AD74412R
     - ch 3 -> device 1 register channel (the displacement) 1 of AD74412R
     - ch 4 -> device 1 register channel (the displacement) 2 of AD74412R
     - ch 5 -> device 1 register channel (the displacement) 3 of AD74412R
     - ch 6 -> device 0 register channel (the displacement) 2 of AD74412R
     - ch 7 -> device 0 register channel (the displacement) 3 of AD74412R
     */

  /* use write_reg()/read_reg() to write/read "channels register" (see above) 
     - addr is the "base" address defined in 'register map' section in OptaAnalogCfg.h
       file
     - and channel is a value between 0 and 8
     So these functions re-map Opta Analog channels to the analog devices (2) 
     Pay attention that these function internally use read_direct_reg/write_direct_reg
     */

  void write_reg(uint8_t addr, uint16_t value, uint8_t ch);
  bool read_reg(uint8_t add, uint16_t &value, uint8_t ch);
  uint8_t get_add_offset(uint8_t ch);
  
  /* use these function to read/write "single register" (see above) */

  bool read_direct_reg(uint8_t device, uint8_t addr, uint16_t &value);
  void write_direct_reg(uint8_t device, uint8_t addr, uint16_t value);

  /* contains all the digital input status */
  void update_alert_mask(int8_t ch);
  void update_alert_status();
  void update_live_status(uint8_t ch);
  void update_live_status();
  void update_adc_value(uint8_t ch);
  void update_adc_diagnostic(uint8_t ch);

  bool is_adc_started(uint8_t device);
  void stop_adc(uint8_t device, bool power_down);
  void start_adc(uint8_t device, bool single_acquisition);
  bool is_adc_updatable(uint8_t device, bool wait_for_conversion);
  bool adc_enable_channel(uint8_t ch, uint16_t &reg);



  /* SPI Sending and receiving messages functions */

  /* init pwm */
  void begin_pwms();
  /* tell if the ADC conversion is in progress */
  bool stAdcIsBusy(uint8_t device);
  /* tell if a channel is assigned to DAC function */
  bool is_dac_used(uint8_t ch);
  bool parse_setup_rtd_channel();
  bool parse_setup_adc_channel();
  bool parse_setup_dac_channel();
  bool parse_setup_di_channel();
  bool parse_setup_high_imp_channel();
  bool parse_get_adc_value();
  bool parse_get_all_adc_value();
  bool parse_set_dac_value();
  bool parse_set_all_dac_value();
  bool parse_get_di_value();
  bool parse_set_pwm_value();
  bool parse_get_rtd_value();
  bool parse_set_rtd_update_rate();
  bool parse_set_led();
  bool parse_get_channel_func();

  void toggle_ldac();

  void reset_dac_value(uint8_t ch);

  FspTimer timer;
  void set_up_timer();
  volatile unsigned int timer_call_num = 0;
  unsigned int timer_timout_ms = OPTA_ANALOG_WATCHTDOG_TIME_ms;
  static void timer_callback(timer_callback_args_t *arg);
  bool use_default_output_values = false;
  void write_dac_defaults(uint8_t ch);
public:
  OptaAnalog();
  void begin() override;
  void update() override;
  int parse_rx() override;

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
  /* NAMING CONVENTION:
   * ------------------
   *  All functions start with the following prefix (each prefix has certain
   *  meaning):
   *  'configure': only set a variable to a value appropriate for that
   *  'send': take all the settings (the variables set with 'configure'
   * function) and send the configuration to the Analog chip. From this point
   * the configuration is actually used. 'update': function to be called
   * periodically (usually to get information form the device but possibly
   * also to set something to the Analog device), the update function read
   * information and put it into a variable 'get' get the value of the
   * variable set with the 'update' function 'set' is used to make "complex"
   * pre-configured configuration and compose some 'configure' functions and
   * 'send' */

  /* ################################################################### */
  /* CONFIGURE CHANNEL FUNCTIONs                                         */
  /* ################################################################### */
  void configureFunction(uint8_t ch, CfgFun_t f, bool write = true);
  void sendFunction(uint8_t ch, CfgFun_t f);
  CfgFun_t getFunction(uint8_t ch);

  /* ################################################################### */
  /* ADC FUNCTIONs                                                       */
  /* ################################################################### */

  /* configure the ADC function */
  void configureAdcMux(uint8_t ch, CfgAdcMux_t m);
  void configureAdcRange(uint8_t ch, CfgAdcRange_t r);
  void configureAdcPullDown(uint8_t ch, bool en);
  void configureAdcRejection(uint8_t ch, bool en);
  void configureAdcDiagnostic(uint8_t ch, bool en);
  void configureAdcDiagRejection(uint8_t ch, bool en);
  void configureAdcMovingAverage(uint8_t ch, uint8_t ma);
  void configureAdcEnable(uint8_t ch, bool en);
  /* send ADC configuration to the device */
  void sendAdcConfiguration(uint8_t ch);
  /* start and stop the ADC conversion */
  void startAdc(bool single = false);
  void stopAdc(bool power_down = false);
  /* read adc values for all the enabled channels */
  void updateAdc(bool wait_for_conversion = false);
  void updateAdcDiagnostics();
  /* get the ADC values */
  uint16_t getAdcValue(uint8_t ch);
  uint16_t getAdcDiagValue(uint8_t ch);

  bool stConvAdcFinished(uint8_t device);
  /* performs a SW reset of the 2 analog devices present on Opta Analog */
  void swAnalogDevReset();
  void sychLDAC();

  /* ##################################################################### */
  /*                         RTD FUNCTIONs                                 */
  /* ##################################################################### */
  /* assign the RTD function to the channel ch, the use_3_w is meant to indicate
   * the use of 3 wires RTD. 3 wires RTD is available only for channels 0 and 1.
   * If this parameter is true for channels different from 0 and 1 it is not
   * used. The configuration is not actually used until the 'send' function is
   * called */
  void configureRtd(uint8_t ch, bool use_3_w, float current);
  /* calculate and update the Rdt values */
  void updateRtd();
  /* get the RTD value */
  float getRtdValue(uint8_t ch);
  /* ##################################################################### */
  /*                         DIN FUNCTIONs                                 */
  /* ##################################################################### */

  void configureDinFilterCompIn(uint8_t ch, bool en);
  void configureDinInvertCompOut(uint8_t ch, bool en);
  void configureDinEnableComp(uint8_t ch, bool en);
  void configureDinDebounceSimple(uint8_t ch, bool en);
  void configureDinScaleComp(uint8_t ch, bool en);
  void configureDinCompTh(uint8_t ch, uint8_t v);
  void configureDinCurrentSink(uint8_t ch, uint8_t v);
  void configureDinDebounceTime(uint8_t ch, uint8_t v);

  void sendDinConfiguration(uint8_t ch);
  void updateDinReadings();
  bool getDinValue(uint8_t ch);

  /* ##################################################################### */
  /*                         GPO FUNCTIONs                                 */
  /* ##################################################################### */

  void configureGpo(uint8_t ch, CfgGpo_t f, GpoState_t state);
  void updateGpo(uint8_t ch);
  void digitalWriteAnalog(uint8_t ch, GpoState_t s);


  /* ##################################################################### */
  /*                         DAC FUNCTIONs                                 */
  /* ##################################################################### */

  /* set the DAC configuration */
  void configureDacCurrLimit(uint8_t ch, CfgOutCurrLim_t cl);
  void configureDacUseSlew(uint8_t ch, CfgOutSlewRate_t sr, CfgOutSlewStep_t r);
  void configureDacDisableSlew(uint8_t ch);
  void configureDacUseReset(uint8_t ch, uint16_t value);
  void configureDacDisableReset(uint8_t ch);
  void configureDacResetValue(uint8_t ch, uint16_t value);
  /* reset the DAC value to the reset DAC value - at the
   * present this function reset all channel configured in a
   * single analog device TODO: verify this behaviour */
  /* write DAC configuration to the device */
  void sendDacConfiguration(uint8_t ch);
  /* used to set the DAC value \*/
  void configureDacValue(uint8_t ch, uint16_t value);
  /* write the DAC value to the device */
  void updateDacValue(uint8_t ch, bool toggle = true);
  /* read the current DAC value used (this can be different
   * from Dac Value due to slew rate settings */
  void updateDacPresentValue(uint8_t ch);
  /* get the current Dac value */
  uint16_t getDacCurrentValue(uint8_t ch);

  /* write the reset value to the device */
  void resetDacValue(uint8_t ch);
  /* update DAC channel if allocated to this function */
  void updateDac(uint8_t ch);

  /* ##################################################################### */
  /*                     ALERT AND DIAGNOSTIC                              */
  /* ##################################################################### */
  void configureAlertMaskRegister(uint8_t device, uint16_t alert);
  void updateAlertStatusRegister(uint8_t device);

  void enableThermalReset(bool en);

  uint8_t getSiliconRevision(uint8_t device);

  void writeReg(uint8_t addr, uint16_t value, int8_t ch) {
    return write_reg(addr, value, ch);
  }
  uint16_t readReg(uint8_t device, uint8_t addr) {
    uint16_t value = 0;
    read_direct_reg(device, addr, value);
    return value;
  }

  /* ##################################################################### */
  /*                                PWMS                                   */
  /* ##################################################################### */
  /* configure the period of the PWM related to channel ch */
  void configurePwmPeriod(uint8_t ch, uint32_t period_us);
  /* configure the pulse of the PWM related to channel ch */
  void configurePwmPulse(uint8_t ch, uint32_t pulse_us);
  /* update PWM, if is not active the Pwm is automatically
   * started */
  void updatePwm(uint8_t ch);
  void updatePwmWithDefault(uint8_t ch);
  /* suspend the PWM */
  void suspendPwm(uint8_t ch);

  /* ##################################################################### */
  /*                                LEDS                                   */
  /* ##################################################################### */

  void updateLedStatus();
  void setLedStatus(uint8_t i);
#ifdef DEBUG_SERIAL
  void displayOaDebugInformation();
  int debugChannelFunction(uint8_t ch);
  void debugAdcConfiguration(uint8_t ch);
  void debugDiConfiguration(uint8_t ch);
  void debugDacFunction(uint8_t ch);
  void debugPrintChannelFunction(uint8_t ch);
  void debugPrintDac();
#endif
#ifdef DEBUG_UPDATE_FW
  void set_led_on(uint8_t l) { led_status |= (1 << l); }
  void set_led_off(uint8_t l) { led_status &= ~(1 << l); }
  void debug_with_leds();
#endif

  void _incrementTimerCallNum() { timer_call_num++; }
  void _resetTimerCallNum() { timer_call_num = 0; }
  bool _timeIsNotForever() { return (timer_timout_ms < 0xFFFF); }
  bool _timerElapsed() { return (timer_call_num >= timer_timout_ms); }
  unsigned int _timerCallNum() { return timer_call_num; }
  void _resetOutputs(bool flag);
};
#endif
#endif
