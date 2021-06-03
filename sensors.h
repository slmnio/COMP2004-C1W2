#ifndef MBED_SLMN_SENSORS
#define MBED_SLMN_SENSORS
#include <string>
#include "mbed.h"
#include "BMP280_SPI.h"

#define BMP_MOSI_PIN PB_5
#define BMP_MISO_PIN PB_4
#define BMP_SCLK_PIN PB_3
#define BMP_CS_PIN   PB_2

struct SensorData {
    float temperature;
    float pressure;
    float light;
    string tempf();
    void print();
};


struct Sensors {
    protected: 
        AnalogIn ldr;
        BMP280_SPI bmp;
    public:
        Sensors() : ldr(PC_0), 
                    bmp(BMP_MOSI_PIN, BMP_MISO_PIN, BMP_SCLK_PIN, BMP_CS_PIN) 
                    {};
        SensorData getData();
};

#endif