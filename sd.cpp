/* COMP2004-W2 - Solomon Cammack (solo) - 10613265 */
/* SD card file: handles the write buffer & SD read/write */

#include "sd.h"
#include "serial.h"
#include "sensors.h"
#include "SDBlockDevice.h"
#include "FATFileSystem.h"

SDBlockDevice sd(PB_5, PB_4, PB_3, PF_3);
// PF_4 / ADC3IN14 / SD_CD is SD detect interrupt

DigitalOut GreenLED(LED2);
void GreenLEDOn() { GreenLED = 1; }
void GreenLEDOff() { GreenLED = 0; }

int sd_write(string data) {
    // Can set mocking to true if the SD cards continue to mess up.
    bool mocking = false;

    if (mocking) {
        // Pretending that the SD writing is doing something
        ThisThread::sleep_for(500ms);
        printf("-- Writing to SD: --\n%s\n----\n", data.c_str());
        ThisThread::sleep_for(5000ms);
    } else {
        // Real SD code
        if (sd.init() != 0) {
            log(true, "SD card init failed");
            sd.deinit();
            return -1;
        }

        FATFileSystem fs("sd", &sd);
        FILE *fp = fopen("/sd/data.txt", "w"); // file pointer
        // mounted 
        GreenLEDOn();

        if (fp == NULL) {
            // file pointer is null if the file can't be accessed
            log(true, "Could not open the file to write to.");
            sd.deinit();
            return -1;
        }

        // close everything up
        fprintf(fp, "%s", data.c_str()); // TODO: replace this with data param
        fclose(fp);
        sd.deinit();
    }

    log(false, "Data written to file.");
    return 0;
}

int FIFO_Buffer::push(SensorData item) {
    // push a piece of data into the buffer
    // printf("[DEBUG] Waiting for semaphore...\n");
    semaphore.acquire();
    this->_buffer.push_back(item);
    semaphore.release();
    return 0;
}

int FIFO_Buffer::size() {
    return this->_buffer.size();
}

SensorData FIFO_Buffer::unshift() {
    SensorData data = this->_buffer.front();
    // printf("unshifted data: %s\n", data.toHumanFormat().c_str());
    this->_buffer.pop_front();
    return data;
}

string FIFO_Buffer::flush() {
    // flush all items and get a string of their human readable contents
    string data = "";
    log(false, "Flushing " + to_string(this->size()) + " items");

    while (!this->_buffer.empty()) {
        string latestData = unshift().toHumanFormat();
        // printf("flush %s\n", latestData.c_str());
        data += latestData + "\n";
    }
    return data;
}

int FIFO_Buffer::sd_flush() {
    // flush buffer onto the SD card 
    // printf("[DEBUG] sd_flush %i\n", this->size());

    // printf("[DEBUG] Waiting for semaphore...\n");
    semaphore.acquire();
    printf("[DEBUG] Received semaphore, SD flushing...\n");
    string data = this->flush();
    semaphore.release();

    int write_attempt = sd_write(data);
    if (write_attempt != 0) {
        GreenLEDOff();
    }
    return write_attempt;
}