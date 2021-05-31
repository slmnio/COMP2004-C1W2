#include "mbed.h"
#include "BMP280.h"
#include "sensors.cpp"

mbed::AnalogIn potentiometer(PA_0);
mbed::AnalogIn ldr(PC_0);


#define BMP280_SDA PB_5;
#define BMP280_ADDR PB_4;
#define BMP280_SCL PB_3;
#define BMP280_CS PB_2;
// I2C bmp_i2c(BMP280_SDI)

// main() runs in its own thread in the OS
int main()
{
    // bmp(BMP280_SDA, BMP280_SCL, 'a');

    // BMP280 bmp280(PB_5, PB_4, PB_3, PB_2); 
    // bmp280.initialize();

    Sensors sensors(&potentiometer, &ldr/*, &bmp*/);
    

    while (true) {
        sensors.getSensorData();
        wait_us(200 * 1000);
    }
    
    return 0;
}

