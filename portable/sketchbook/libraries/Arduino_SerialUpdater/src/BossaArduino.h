/*
  BossaArduino.h
  Copyright (c) 2023 Arduino SA.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _BOSSAA_H
#define _BOSSAA_H

#include "Arduino.h"
#include "Flasher.h"
#include "Samba.h"
#include "SerialPort.h"

class BossacSerialPort : public SerialPort {
public:
  BossacSerialPort(const std::string &name, HardwareSerial &serial)
      : SerialPort(name), _serial{&serial} {}
  virtual ~BossacSerialPort() {}

  bool open(int baud, int data, Parity parity, StopBit stop) {
    _serial->begin(baud);
    return true;
  }
  void close() {}
  bool initcmd() { return true; }
  bool endcmd() { return true; }
  bool isUsb() { return true; }
  int read(uint8_t *data, int size) { return _serial->readBytes(data, size); }
  int write(const uint8_t *data, int size) {
    return _serial->write(data, size);
  }
  int get() { return _serial->read(); }
  int put(int c) {
    _serial->write(c);
    flush();
    return 0;
  }
  bool timeout(int millisecs) {
    _serial->setTimeout(millisecs);
    return true;
  }
  void flush() { _serial->flush(); }
  void setDTR(bool dtr) {}
  void setRTS(bool rts) {}

private:
  HardwareSerial *_serial;
};

class BossaArduinoObserver : public FlasherObserver {
public:
  BossaArduinoObserver() {}
  virtual ~BossaArduinoObserver() {}

  virtual void onStatus(const char *message,
                        ...){/* Print something on status change */};
  virtual void onProgress(int num, int div){/* Print something on progress */};
};

class BossaArduino {
public:
  BossaArduino(FlasherObserver &observer) : _observer(observer) {}
  virtual ~BossaArduino() {}

  virtual int connect(HardwareSerial &serial);
  virtual void flash(const char *file_path);

protected:
  Samba _samba;
  FlasherObserver &_observer;
};

#endif
