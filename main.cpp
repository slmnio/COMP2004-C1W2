#include "mbed.h"
// #include "BMP280_SPI.h"
#include "sensors.h"

int main() {

    Sensors sensors;
    // bmp.initialize();

    while (true) {
        sensors.getData();
    }
    return 0;
}