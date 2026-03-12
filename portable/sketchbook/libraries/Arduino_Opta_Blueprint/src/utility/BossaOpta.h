/* -------------------------------------------------------------------------- */
/* FILE NAME:   BossaOpta.h
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240116
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#ifndef BOSSAOPTA_INCLUDED
#define BOSSAOPTA_INCLUDED

#include "OptaController.h"
#include <stdint.h>
#include <string>
#ifdef ARDUINO_OPTA

#include "Arduino.h"
#include <Arduino_SerialUpdater.h>

class BossaOptaObserver : public FlasherObserver {
public:
  BossaOptaObserver() : _lastTicks(-1) {}
  virtual ~BossaOptaObserver() {}

  virtual void onStatus(const char *message, ...){};
  virtual void onProgress(int num, int div);

private:
  int _lastTicks;
};

class BossaOpta {
public:
  BossaOpta(FlasherObserver &observer)
      : ctrl(nullptr), initial_num_of_devices(0), block_updates(false) {}
  virtual ~BossaOpta() {}

  /* pass the controller and the device index that has to be updated
   * the reboot option is due to the fact that sometimes we need to update
   * devices that are have just the bootloader (this not the normal way
   * but during development it can happen), in this case the device cannot
   * answer to the reboot function simply because it is not there */
  bool begin(HardwareSerial &serial, Controller *c, uint8_t device,
             bool reboot = true);
  bool flash(const uint8_t *const fw, uint32_t size);
  void reset();
  std::string version();

  Samba &getSamba() { return _samba; }
  uint16_t calcCrc(const uint8_t *const b, uint32_t sz);

private:
  // uint16_t calcCrc(const uint8_t *const b, uint32_t sz);
  Controller *ctrl;
  Samba _samba;
  std::unique_ptr<Flash> _flash;
  uint8_t initial_num_of_devices;
  bool block_updates;
};

extern BossaOpta BOSSA;

#endif // ARDUINO_OPTA

#endif // BOSSAOPTA_INCLUDED
