/* COMP2004-W2 - Solomon Cammack (solo) - 10613265 */
/* SD card file: handles the write buffer & SD read/write */

#include "sd.h"
#include "serial.h"
#include "sensors.h"
#include "SDBlockDevice.h"
#include "FATFileSystem.h"

SDBlockDevice sd(PB_5, PB_4, PB_3, PF_3);
// PF_4 / ADC3IN14 / SD_CD is SD detect interrupt

int sd_write(string data) {
    // printf("-- Writing to SD: --\n%s\n----\n", data.c_str());


    if (sd.init() != 0) {
        log(true, "SD card init failed");
        return -1;
    }

    FATFileSystem fs("sd", &sd);
    FILE *fp = fopen("/sd/data.txt", "w"); // file pointer

    if (fp == NULL) {
        log(true, "Could not open the file to write to.");
        sd.deinit();
        return -1;
    }

    fprintf(fp, "%s", data.c_str()); // TODO: replace this with data param
    fclose(fp);
    log(false, "Data written to file.");
    sd.deinit();
    return 0;
}

int FIFO_Buffer::push(SensorData item) {
    // add to buffer
    // printf("buffer req: %s\n", item.c_str());
    // printf("current size: %i\n", sizeof(FIFO_Buffer::_buffer));
/*
    if (FIFO_Buffer::_buffer[FIFO_Buffer::index] == 0)  {
        // overlap
        log(true, "Buffer overlap");
        return -1;
    }
*/

    // printf("[DEBUG] Waiting for semaphore...\n");
    semaphore.acquire();
    // printf("[DEBUG] Received semaphore.\n");

    this->_buffer.push_back(item);
    // printf("[DEBUG] ->_: _ size is %i\n", size()); // works
    // printf("[DEBUG] ->_: buffer size is %i\n", buffer.size()); // doesn't
    // printf("[DEBUG] ->push: buffer size is %i\n", this->_buffer.size()); // works
    // printf("[DEBUG] buffer->buffer size is %i\n", buffer._buffer.size()); // doesn't
    // printf("[DEBUG] buffer is now size %i\n", _buffer.size());

    // FIFO_Buffer::_buffer[FIFO_Buffer::index] = item;
    // FIFO_Buffer::index++;
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
    string data;
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

    sd_write(data);

    // printf("%s", flush().c_str());


    return -1;
}