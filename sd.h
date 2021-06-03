#ifndef MBED_SLMN_SD
#define MBED_SLMN_SD

#include "mbed.h"
#include <string>

int sd_write(string data);

struct FIFO_Buffer {
    string _buffer[64];
    int index = 0;
    int push(string item);
    int unshift(string item);
};

static FIFO_Buffer buffer;

#endif