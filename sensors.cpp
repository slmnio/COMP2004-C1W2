#include "BMP280_SPI.h"
#include "mbed.h"
#include "sensors.h"

#define BMP280_MOSI_PIN PB_5
#define BMP280_MISO_PIN PB_4
#define BMP280_SCLK_PIN PB_3
#define BMP280_CS_PIN   PB_2


void Sensors::getData() {
     // =operator -> read();
    float _p = Sensors::potentiometer;
    float _l = Sensors::ldr;

    // casting to int because mbed's float printf doesn't work
    printf("potentiometer: %i | ldr: %i\n", int (_p * 1000), int (_l * 1000));
}