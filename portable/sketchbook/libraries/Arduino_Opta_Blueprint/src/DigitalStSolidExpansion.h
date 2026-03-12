/* -------------------------------------------------------------------------- */
/* FILE NAME:   DigitalStSolidExpansion.h
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

#ifndef DIGITALSTSOLIDEXPANSION
#define DIGITALSTSOLIDEXPANSION
#include "DigitalExpansion.h"

namespace Opta {
class DigitalStSolidExpansion : public DigitalExpansion {
public:
  DigitalStSolidExpansion();
  virtual ~DigitalStSolidExpansion();
  DigitalStSolidExpansion(Expansion &other);
  static Expansion *makeExpansion();
  static std::string getProduct();

protected:
private:
};
} // namespace Opta
#endif
