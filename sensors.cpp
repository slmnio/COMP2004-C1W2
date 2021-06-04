/* COMP2004-W2 - Solomon Cammack (solo) - 10613265 */
/* Sensors: reads and formats sensor data */

#include "BMP280_SPI.h"
#include "mbed.h"
#include "sensors.h"
#include <string>
#include "serial.h"


string SensorData::tempf() {
    // nicer way of showing off the temperature
    char _str[64];
    sprintf(_str, "%.1fC", temperature);
    return _str;
}
void SensorData::print() {
    // helper function used sometimes
    printf("%s", this->toHumanFormat().c_str());
}
string SensorData::toHumanFormat() {
    // human readable (different to url encoded, could be different to SD write)
    char _str[256];
    sprintf(_str, "time, %.1f%%, %s, %.2f mbar", light * 100, tempf().c_str(), (pressure));
    return _str;
}
string SensorData::urlEncode() {
    // not a true encode but good enough for the symbols we might encounter
    char str[256] = "";
    sprintf(str, "light=%.3f&temperature=%.1f&pressure=%.1f", light, temperature, pressure);
    return str;
}

SensorData Sensors::getData() {
    // this is the method that reads all the pins and gets the data
    float _l = Sensors::ldr.read();
    float _t = Sensors::bmp.getTemperature();
    float _p = Sensors::bmp.getPressure();

    log(false, "Sensor data read");

    lastData = {
        _t, _p, _l
    };
    return lastData;
}