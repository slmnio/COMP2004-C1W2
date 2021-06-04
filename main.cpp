#include "mbed.h"
// #include "BMP280_SPI.h"
#include "sensors.h"
#include "sd.h"

Sensors sensors;
EventQueue queue;
Thread t_data;
Thread t_sd;

FIFO_Buffer buffer;

void collectData() {
    // printf("[DEBUG] Data collecting...\n");
    SensorData data = sensors.getData();

    // buffer.push_event(data);
    // t_sd.start(buffer.push(d/ata));
    // printf("[DEBUG] collect: buffer size is %i\n", buffer.size());
    buffer.push(data);
    // data.print();
}

void buffer_sd_flush() {
    printf("[DEBUG] SD buffer flushing...\n");
    printf("[DEBUG] buffer size is %i\n", buffer.size());
    buffer.sd_flush();
}


void dataThread() {
    printf("[DEBUG] Data thread started\n");
    // sd_main();
    // t_sd.start(callback(&sd_main));

    queue.call_every(1s, collectData);
    queue.dispatch();

}

EventQueue sd_queue;

void sd_main() {
    printf("[DEBUG] SD thread started\n");
    // should be 1min+, lower here for testing.
    sd_queue.call_every(20s, &buffer_sd_flush);
    sd_queue.dispatch();
}

int main() {

    t_data.start(callback(&dataThread));
    t_sd.start(callback(&sd_main));

    return 0;
}
