#include <iostream>
#include "mbed.h"
#include "BMP280.h"

class Sensors {
    protected:
        AnalogIn *potentiometer;
        AnalogIn *ldr;
        //BMP280 *bmp;

    public:
        Sensors(AnalogIn *potentiometer, AnalogIn *ldr/*, BMP280 *bmp*/) {
            this->potentiometer = potentiometer;
            this->ldr = ldr;
            // this->bmp = bmp;
        }

        void getSensorData() {
            double _p = potentiometer->read();
            double _l = ldr->read();

            /*double _temp = bmp->getTemperature();
            double _pres = bmp->getPressure();*/

            //printf("potentiometer: %i | ldr: %i\r\ntemp: %i, pres: %i\n", int (_p * 1000), int (_l * 1000), int (_temp * 1000), int (_pres * 1000));
            printf("potentiometer: %i | ldr: %i\n", int (_p * 1000), int (_l * 1000));
        }
};