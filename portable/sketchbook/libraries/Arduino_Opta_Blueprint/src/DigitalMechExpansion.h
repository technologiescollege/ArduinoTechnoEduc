/* -------------------------------------------------------------------------- */
/* FILE NAME:   DigitalMechExpansion.h
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

#ifndef DIGITALMECHEXPANSION
#define DIGITALMECHEXPANSION
#include "DigitalExpansion.h"

namespace Opta {

class DigitalMechExpansion : public DigitalExpansion {
public:
  DigitalMechExpansion();
  virtual ~DigitalMechExpansion();
  DigitalMechExpansion(Expansion &other);
  static Expansion *makeExpansion();
  static std::string getProduct();

protected:
private:
};

} // namespace Opta
#endif
