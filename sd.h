/* COMP2004-W2 - Solomon Cammack (solo) - 10613265 */

#ifndef MBED_SLMN_SD
#define MBED_SLMN_SD

#include "mbed.h"
#include "sensors.h"
#include <string>
#include <deque>

int sd_write(string data);

struct FIFO_Buffer {
    // I'm not sure if deque is allowed, but I'll build around it and can replace it if needed.
    std::deque<SensorData> _buffer;
    Semaphore semaphore{1};
    int push(SensorData item);
    SensorData unshift();

    string flush();
    int sd_flush();
    int size();
};

void sd_core_main();

#endif