#ifndef MBED_SLMN_SENSORS
#define MBED_SLMN_SENSORS
#include "mbed.h"

struct Sensors {
    protected: 
        AnalogIn potentiometer;
    public:
        Sensors() : potentiometer(PA_0) {};
        // Sensors(PinName potentiometerPin);
        void getData();
};

#endif