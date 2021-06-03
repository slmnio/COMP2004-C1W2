#ifndef MBED_SLMN_SENSORS
#define MBED_SLMN_SENSORS
#include "mbed.h"

struct Sensors {
    protected: 
        AnalogIn potentiometer;
        AnalogIn ldr;
    public:
        Sensors() : potentiometer(PA_0), ldr(PC_0) {};
        // Sensors(PinName potentiometerPin);
        void getData();
};

#endif