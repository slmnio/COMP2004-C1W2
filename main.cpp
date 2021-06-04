/* COMP2004-W2 - Solomon Cammack (solo) - 10613265 */
/* Main: co-ordinate and launch threads */

#include "mbed.h"
#include "sensors.h"
#include "sd.h"
#include "network.h"
#include "serial.h"

Sensors sensors;
Thread t_data;
Thread t_sd;
Thread t_net;
//Thread t_serial;

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

Thread t_serial_control;
BufferedSerial pc_serial(USBTX, USBRX);

int run_command(char command[]) {
    printf("[DEBUG] RUN COMMAND %s\n", command);
    if (strcmp(command, "READ NOW") == 0) {
        printf("[COMMAND] Latest record: %s\n", sensors.lastData.toHumanFormat().c_str());
        return 0;
    }
    if (strcmp(command, "SD F") == 0) {
        printf("[COMMAND] Flushing SD card\n");
        sd_queue.call(buffer_sd_flush);
        return 0;
    }
    if (strcmp(command, "NET UPDATE") == 0) {
        printf("[COMMAND] Forcing a new network update\n");
        network_queue.call(sendDataToServer);
        return 0;
    }
    printf("[COMMAND] Unrecognised command\n");
    return -1;
}

void serial_control() {
    
    pc_serial.set_baud(9600);
    pc_serial.set_format(
        /* bits */ 8,
        /* parity */ BufferedSerial::None,
        /* stop bit */ 1
    );
    char command[64] = {0};
    char buf[32] = {0};

    while (true) {
         if (uint32_t num = pc_serial.read(buf, sizeof(buf))) {
            // Echo the input back to the terminal.
            pc_serial.write(buf, num);

            switch (buf[0]) {
                case 0x7f:
                    // backspace
                    // something with a \0;
                    break;
                case 0x0d:
                    // enter
                    run_command(command);
                    memset(command, 0, sizeof command);
                    break;
                default:
                    strcat(command, buf);
            }
        }
    }
}


int main() {
    t_data.start(callback(&dataThread));
    t_sd.start(callback(&sd_main));
    t_net.start(callback(&network_main));
    serial_external_main();
    t_serial_control.start(callback(&serial_control));
    
    return 0;
}


