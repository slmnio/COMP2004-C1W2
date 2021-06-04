/* COMP2004-W2 - Solomon Cammack (solo) - 10613265 */
/* Serial communications: handles logging and serial interface (#12) */

#include "serial.h"
#include "mbed.h"

EventQueue serial_queue;

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

DigitalOut redLED(LED3);

void ledHigh() {
    redLED = 1;
}

void ledLow() {
    redLED = 0;
}

void triggerRedLED() {
    // something about a red LED here
    serial_queue.call(&ledHigh);
    serial_queue.call_in(3s, &ledLow);
}

Thread t_serial;
void serial_external_main() {
    t_serial.start(callback(&serial_queue, &EventQueue::dispatch_forever));
}