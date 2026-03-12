/* -------------------------------------------------------------------------- */
/* FILE NAME:   DigitalExpansion.h
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

#ifndef DIGITALEXPANSION
#define DIGITALEXPANSION
#include "DigitalCommonCfg.h"
#include "DigitalExpansionsAddresses.h"
#include "ExpansionOperations.h"
#include "OptaBluePrintCfg.h"
#include "OptaController.h"
#include "OptaDigitalProtocol.h"
#include "OptaExpansion.h"
#include <stdint.h>
namespace Opta {

class DigitalExpansionSetup {
public:
  DigitalExpansionSetup() {
    default_digital_out = 0;   // all switched off
    default_timeouts = 0xFFFF; // disable timeout
  }
  uint8_t default_digital_out;
  uint16_t default_timeouts;
};

class DigitalExpansion : public Expansion {
protected:
  bool verify_address(unsigned int add) override;

  /* set digital input */
  uint8_t msg_set_di();
  bool parse_ans_set_di();
  /* get digital input */
  uint8_t msg_get_di();
  bool parse_ans_get_di();
  /* msg get analog input */
  uint8_t msg_get_ai();
  bool parse_ans_get_ai();

  uint8_t msg_set_default_values();

  /* msg get all analog input */
  uint8_t msg_get_all_ai();
  bool parse_ans_get_all_ai();

  static uint16_t timeouts[OPTA_CONTROLLER_MAX_EXPANSION_NUM];
  static uint8_t defaults[OPTA_CONTROLLER_MAX_EXPANSION_NUM];
  static uint8_t last_expansion_output[OPTA_CONTROLLER_MAX_EXPANSION_NUM];

public:
  DigitalExpansion();
  DigitalExpansion(Expansion &other);
  unsigned int execute(uint32_t what) override;
  static Expansion *makeExpansion();
  static std::string getProduct();
  /* set the status of a digital output
   * if update is false the new status of the pin is set on a local variable
   * but not sent to the expansion
   * this allow to set more pins status and then set them at once sending the
   * update to the expansion
   * if update is true the new status is immediately transferred to the
   * expansion
   * please note that all digital outputs are transferred in 1 single I2C
   * transaction */
  void digitalWrite(int pin, PinStatus st, bool update = false);

  /* get the status digital output has been set */
  PinStatus digitalOutRead(int pin);

  /* get the status of a digital input
   * if update is false the new status of the pin is get from a local variable
   * which may not be aligned with the status of the actual pin's status
   * this allow to use coherent photograph of the pin status at a certain moment
   * in time
   * if update is true the status of the pin is first updated from the
   * expansion via i2c communication
   * please note that all digital inputs are transferred in 1 single I2C
   * transaction */
  PinStatus digitalRead(int pin, bool update = false);

  /* get the status of a pin as an analog pin
   * if the update is true an actual i2c communication is performed to get
   * the analog pin status from the expansion
   * if the update is false no communication is performed and so the last
   * updated value is returned
   * please note that when using this function just 1 analog value is
   * transferred */
  int analogRead(int pin, bool update = true);
  /* get the voltage of the analog pin */
  float pinVoltage(int pin, bool update = true);

  /* this function will update the local status of the digital input to the
   * actual status present on the expansion */
  void updateDigitalInputs();

  /* this function will update the local status of the analog inputs (all of
   * them) the actual analog status present on the expansion */
  void updateAnalogInputs();

  /* this function will update the digital outputs of the expansion with the
   * status set by digitalWrite (this make sense if we update more than one
   * digital outputs with digitalWrite and its last argument false and then
   * we update the actual value with this function)*/
  void updateDigitalOutputs();

  void setProductData(uint8_t *data, uint8_t len);
  void setIsMechanical();
  void setIsStateSolid();

  void write(unsigned int address, unsigned int value) override;
  bool read(unsigned int address, unsigned int &value) override;
  static uint8_t msgDefault(Controller *ptr, uint8_t device);
  static void startUp(Controller *ptr);
  static void setDefault(Controller &ptr, uint8_t device, uint8_t bit_mask,
                         uint16_t timeout);
  static uint8_t calcDefault(bool p0, bool p1, bool p2, bool p3, bool p4,
                             bool p5, bool p6, bool p7);
};

} // namespace Opta
#endif
