/* -------------------------------------------------------------------------- */
/* FILE NAME:   BossaOpta.cpp
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

#include <cstdint>
#include <cstring>
#include <stdint.h>

#ifdef ARDUINO_OPTA

#if __has_include("utility/BossaOpta.h")

#include "Arduino.h"
#include "utility/BossaOpta.h"
#include <Arduino_DebugUtils.h>

void BossaOptaObserver::onProgress(int num, int div) {

  int ticks;
  int bars = 30;
  ticks = num * bars / div;

  if (ticks == _lastTicks) {
    return;
  }

  Debug.newlineOff();
  DEBUG_INFO("\r[");
  while (ticks-- > 0) {
    DEBUG_INFO("=");
    bars--;
  }
  while (bars-- > 0) {
    DEBUG_INFO(" ");
  }
  Debug.newlineOn();

  DEBUG_INFO("] %d%% (%d/%d pages)", num * 100 / div, num, div);

  _lastTicks = 0;
}

BossaOptaObserver obs;

bool BossaOpta::begin(HardwareSerial &serial, Controller *c, uint8_t device,
                      bool reboot) {
  bool rv = true;
  ctrl = c;
  SerialPort::Ptr port(new BossacSerialPort("bossac", serial));
  _samba.setDebug(false);

  if (ctrl != nullptr && block_updates == false) {

    initial_num_of_devices = ctrl->getExpansionNum();

    if (reboot == false) {
      /* special case when device have only bootloader */
      initial_num_of_devices = 1;
    }

    if (device < initial_num_of_devices) {
      if (reboot) {
        DEBUG_INFO("Rebooting device...");
        if (ctrl->rebootExpansion(device)) {
          DEBUG_INFO(" OK!");
          DEBUG_INFO("Connecting to bootloader...");
          if (!_samba.connect(port, 115200)) {
            DEBUG_INFO(" FAILED connection");

            rv = false;
          }
        } else {
          DEBUG_INFO(" FAILED reboot!");
          rv = false;
        }
      } else {
        if (!_samba.connect(port, 115200)) {
          rv = false;
        }
      }
    } else {
      DEBUG_INFO(" FAILED! (wrong device)");
      rv = false;
    }
  } else {
    DEBUG_INFO(" FAILED! (blocked)");
    rv = false;
  }

  return rv;
}

std::string BossaOpta::version() { return _samba.version(); }
void BossaOpta::reset() {
  int attempts = 0;
  _samba.reset();
  /* this time is critical, we must be sure that the expansion has
   * reset and that all the process to inform the controller do again
   * the discover process */
  delay(10000);
  uint8_t exp_num = 0;
  if (ctrl != nullptr) {
    do {
      ctrl->checkForExpansions();
      delay(1000);
      exp_num = ctrl->getExpansionNum();
      attempts++;
    } while (exp_num != initial_num_of_devices && attempts < 5);

    if (attempts >= 5) {
      /* this is a very critical event, any other update request will be ignored
       */
      block_updates = true;
    }
  }
}

uint16_t BossaOpta::calcCrc(const uint8_t *const b, uint32_t sz) {
  uint16_t crc = 0;
  for (uint32_t i = 0; i < sz; i++) {
    crc = _samba.checksumCalc(*(b + i), crc);
  }
  return crc;
}

bool BossaOpta::flash(const uint8_t *const fw, uint32_t sz) {
  bool rv = true;
  if (_samba.canWriteBuffer() && block_updates == false) {
    int32_t _size = sz;
    uint32_t offset = 0;
    uint32_t bufferSize = _samba.writeBufferSize();
    uint8_t *buffer = new uint8_t[bufferSize];
    uint8_t numPages = (sz / bufferSize) + 1;
    uint8_t page = 0;
    while (_size > 0 && buffer != nullptr) {
      page++;
      obs.onProgress(page, numPages);

      uint32_t to_be_sent = ((uint32_t)_size > bufferSize) ? bufferSize : _size;
      memset(buffer, 0x00, bufferSize);
      memcpy(buffer, fw + offset, to_be_sent);
      uint16_t crc = calcCrc(buffer, bufferSize);

      /* samba works in 2 steps, first the data are downloaded into a buffer
       * in the memory of the bootloader so the
       * - first parameter is the address in the buffer
       * - second parameter is the buffer to be transferred to the bootloader
       * - third parameter is the size of the buffer to be transferred */
      _samba.write(0, buffer, bufferSize);
      delay(10);
      /* samba write buffer tells the bootloader to write the buffer that
       * was previously transferred using the write above to the flash
       * - the first argument is the starting index in the buffer to get data
       *   from
       * - the second argument is the destination address in the flash
       * - the third argument is the size of the data to be written
       *   write performs 2 transfers (first the starting index and then the
       *   offset and the buffer size)*/
      _samba.writeBuffer(0, offset, bufferSize);
      delay(10);
      uint16_t rx_crc = _samba.checksumBuffer(offset, bufferSize);

      if (rx_crc == crc) {
        _size -= bufferSize;
        offset += bufferSize;
      } else {
        rv = false;
        break;
      }
    }
    delete[] buffer;
  } else {
    /* happens on very old prototype with a hardware problem that prevent the
     * serial to work */
    return false;
  }
  return rv;
}

BossaOpta BOSSA(obs);

#endif
#endif
