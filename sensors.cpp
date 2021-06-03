#include "BMP280_SPI.h"
#include "mbed.h"
#include "sensors.h"
#include <string>
#include "serial.h"


string SensorData::tempf() {
    int scale = 10;
    return to_string(int(floor(temperature * scale) / scale)) + "." + to_string(int((temperature * scale) - (floor(temperature) * 10))) + "C";
}
void SensorData::print() {
    // casting to int because mbed's float printf doesn't work
    printf("ldr: %i | temp: %s | pres: %i\n", int (light * 1000), tempf().c_str(), int (pressure * 1000));
}
string SensorData::toHumanFormat() {
    char* _str;
    sprintf(_str, "time, %i, %s, %i", int (light * 1000), tempf().c_str(), int(pressure * 1000));
    printf("time, %i, %s, %i\n", int (light * 1000), tempf().c_str(), int(pressure * 1000));
    printf("str: %s\n", _str);
    return _str;
}

SensorData Sensors::getData() {

    float _l = Sensors::ldr;
     // =operator -> read();
    float _t = Sensors::bmp.getTemperature();
    float _p = Sensors::bmp.getPressure();

    log(false, "Sensor data read\n");

    return {
        _t, _p, _l
    };
}