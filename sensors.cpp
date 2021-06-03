#include "BMP280_SPI.h"
#include "mbed.h"
#include "sensors.h"

#define BMP280_MOSI_PIN PB_5
#define BMP280_MISO_PIN PB_4
#define BMP280_SCLK_PIN PB_3
#define BMP280_CS_PIN   PB_2


void Sensors::getData() {
    float _p = Sensors::potentiometer;
    printf("potentiometer: %i\n", int (_p * 1000));
}