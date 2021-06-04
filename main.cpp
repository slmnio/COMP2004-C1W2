/* COMP2004-W2 - Solomon Cammack (solo) - 10613265 */
/* Main: co-ordinate and launch threads */

#include "mbed.h"
#include "sensors.h"
#include "sd.h"
#include "network.h"

Sensors sensors;
Thread t_data;
Thread t_sd;
Thread t_net;

FIFO_Buffer buffer;

void collectData() {
    // collect data from sensors
    SensorData data = sensors.getData();

    // could be a condition here to only push to buffer if SD card is connected
    buffer.push(data);
}

void buffer_sd_flush() {
    // Flushing the buffer means emptying it and creating a string with the formatted emptied data to be saved.
    printf("[DEBUG] Flushing SD buffer, size is %i\n", buffer.size());
    buffer.sd_flush();
}

void sendDataToServer() {
    // every x seconds this is called. get data from sensors and pass it to the server
    // get sensor data
    string data = sensors.lastData.urlEncode();
    // send data to server
    send_data(data);
}

EventQueue queue;
void dataThread() {
    // Collects data every second in this thread.
    // Thread only gets data then pushes it, no further operations.
    queue.call_every(1s, collectData);
    queue.dispatch();
}

EventQueue sd_queue;
InterruptIn SD_Inserted(PF_4);
void sd_main() {
    // EventQueue to flush the SD card buffer
    SD_Inserted.rise(sd_queue.event(buffer_sd_flush));
    sd_queue.call_every(1min, &buffer_sd_flush);
    sd_queue.dispatch();
}


EventQueue network_queue;
void network_main() {  
    network_queue.call_every(10s, sendDataToServer);
    network_queue.dispatch();
}

int main() {
    t_data.start(callback(&dataThread));
    t_sd.start(callback(&sd_main));
    t_net.start(callback(&network_main));
    
    return 0;
}


