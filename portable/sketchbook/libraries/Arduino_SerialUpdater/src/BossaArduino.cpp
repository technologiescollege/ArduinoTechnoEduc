/*
  BossaArduino.cpp
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
#ifdef ARDUINO

#include <string>
#include <exception>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "BossaArduino.h"

extern "C" void usleep(long i) {
    delayMicroseconds(i);
}


int BossaArduino::connect(HardwareSerial& serial) {

    SerialPort::Ptr port(new BossacSerialPort("bossac", serial));
    _samba.setDebug(true);

    if(!_samba.connect(port, 230400)) {
        return 0;
    }
    return 1;
}

void BossaArduino::flash(const char* file_path) {

    Device device(_samba);
    device.create();

    Flasher flasher(_samba, device, _observer);
    flasher.erase(0);
    delay(100);
    flasher.write(file_path);
}

#endif /* ARDUINO */
