/* COMP2004-W2 - Solomon Cammack (solo) - 10613265 */
/* Serial communications: handles logging and serial interface (#12) */

#include "serial.h"

void log(bool isCritical, string message) {
    /* if logging is enabled, log to serial */

    if (isCritical) {
        triggerRedLED();
        logToSerial("CRITICAL", message);
    } else if (loggingEnabled) {
        logToSerial("LOG", message);
    }
}

void logToSerial(string prefix, string message) {
    // actual call here
    printf("[%s] %s\n", prefix.c_str(), message.c_str());
}

void triggerRedLED() {
    // something about a red LED here
}
