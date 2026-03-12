/* -------------------------------------------------------------------------- */
/* FILE NAME:   DigitalMechExpansion.cpp
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20240209
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */
#ifdef ARDUINO_OPTA
#include "DigitalMechExpansion.h"
namespace Opta {
DigitalMechExpansion::DigitalMechExpansion() {}

DigitalMechExpansion::~DigitalMechExpansion() {}

Expansion *DigitalMechExpansion::makeExpansion() {
  return new DigitalMechExpansion();
}

std::string DigitalMechExpansion::getProduct() {
  std::string rv(OPTA_DIGITAL_MECH_DESCRIPTION);
  return rv;
}

DigitalMechExpansion::DigitalMechExpansion(Expansion &other) {
  DigitalMechExpansion &dme = (DigitalMechExpansion &)other;

  type = EXPANSION_NOT_VALID;
  i2c_address = 0;
  ctrl = other.getCtrl();
  index = 255;

  if (other.getType() == EXPANSION_OPTA_DIGITAL_MEC) {
    iregs = dme.iregs;
    fregs = dme.fregs;
    type = other.getType();
    i2c_address = other.getI2CAddress();
    ctrl = other.getCtrl();
    index = other.getIndex();
  }
}

} // namespace Opta
#endif
