//
// Created by Szvetlin Tanyi on 2020. 04. 06.
//

#ifndef ARROWHEADESP_UTIL_H
#define ARROWHEADESP_UTIL_H

#include <Arduino.h>

#define DEBUG

#ifdef DEBUG
#define debugPrint(x) Serial.print(x) //debug on
#define debugPrintln(x) Serial.println(x) //debug on
#else
#define debugPrint(x) {;} //debug off
#define debugPrintln(x) {;} //debug off
#endif

#endif //ARROWHEADESP_UTIL_H
