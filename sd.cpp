#include "sd.h"
#include "serial.h"

int FIFO_Buffer::push(string item) {
    // add to buffer
    printf("buffer req: %s\n", item.c_str());
    printf("current size: %i\n", sizeof(FIFO_Buffer::_buffer));

    if (!FIFO_Buffer::_buffer[FIFO_Buffer::index].empty())  {
        // overlap
        log(true, "Buffer overlap");
        return -1;
    }

    FIFO_Buffer::_buffer[FIFO_Buffer::index] = item;
    FIFO_Buffer::index++;
    return 0;
}