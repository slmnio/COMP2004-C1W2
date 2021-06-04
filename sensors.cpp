/* COMP2004-W2 - Solomon Cammack (solo) - 10613265 */
/* Sensors: reads and formats sensor data */

#include "BMP280_SPI.h"
#include "mbed.h"
#include "sensors.h"
#include <string>
#include "serial.h"


string SensorData::tempf() {
    char _str[64];
    sprintf(_str, "%.1fC", temperature);
    return _str;
}
void SensorData::print() {
    // casting to int because mbed's float printf doesn't work
    printf("%s", this->toHumanFormat().c_str());
    // printf("ldr: %i | temp: %s | pres: %i\n", int (light * 1000), tempf().c_str(), int (pressure));
}
string SensorData::toHumanFormat() {
    char _str[256];
    // sprintf(_str, "time, %i, %s, %i mbar", int (light * 1000), tempf().c_str(), int(pressure));
    sprintf(_str, "time, %.1f%%, %s, %.2f mbar", light * 100, tempf().c_str(), (pressure));
    // printf("time, %i, %s, %i\n", int (light * 1000), tempf().c_str(), int(pressure * 1000));
    // printf("str: %s\n", _str);
    return _str;
}
string SensorData::urlEncode() {
    char str[256] = "";
    sprintf(str, "light=%.3f&temperature=%.1f&pressure=%.1f", light, temperature, pressure);
    return str;
}

SensorData Sensors::getData() {
    float _l = Sensors::ldr.read();
    float _t = Sensors::bmp.getTemperature();
    float _p = Sensors::bmp.getPressure();

    log(false, "Sensor data read");

    lastData = {
        _t, _p, _l
    };
    return lastData;
}