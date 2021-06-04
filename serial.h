/* COMP2004-W2 - Solomon Cammack (solo) - 10613265 */

#ifndef MBED_SLMN_SERIAL
#define MBED_SLMN_SERIAL

#include "mbed.h"
#include "string"

static bool loggingEnabled = true;

void log(bool isCritical = false, string message = "");
void logToSerial(string prefix, string message);

void triggerRedLED();


#endif