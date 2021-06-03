#include "BMP280_SPI.h"
#include "mbed.h"
#include "sensors.h"
#include <string>


string SensorData::tempf() {
    return to_string(int (floor(temperature))) + "." + to_string(int (temperature / floor(temperature))) +"C";
}
void SensorData::print() {
    // casting to int because mbed's float printf doesn't work
    printf("ldr: %i | temp: %s | pres: %i\n", int (light * 1000), tempf().c_str(), int (pressure * 1000));
}

SensorData Sensors::getData() {

    float _l = Sensors::ldr;
     // =operator -> read();
    float _t = Sensors::bmp.getTemperature();
    float _p = Sensors::bmp.getPressure();

    return {
        _t, _p, _l
    };
}