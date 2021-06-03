#include "mbed.h"
// #include "BMP280_SPI.h"
#include "sensors.h"

Sensors sensors;
EventQueue queue;
Thread t_data;

void collectData() {
    SensorData data = sensors.getData();
    // data.print();
}

void dataThread() {
    queue.call_every(1s, collectData);
    queue.dispatch();
}


int main() {

    t_data.start(callback(&dataThread));

    return 0;
}
