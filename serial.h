#ifndef MBED_SLMN_SERIAL
#define MBED_SLMN_SERIAL

#include "mbed.h"
#include "string"

bool loggingEnabled = false;

void log(bool isCritical = false, string message = "");
void logToSerial(string prefix, string message);

void triggerRedLED();


#endif