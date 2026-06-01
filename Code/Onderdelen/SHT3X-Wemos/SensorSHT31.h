#ifndef SENSOR_SHT31_H
#define SENSOR_SHT31_H

#include <Wire.h>
#include "Adafruit_SHT31.h"

class SensorSHT31
{
private:
    Adafruit_SHT31 sht31;
    float temperature;
    float humidity;

public:
    SensorSHT31();

    bool begin(uint8_t address = 0x44);
    void update();

    float getTemperature();
    float getHumidity();

    void printData();
    void run();
};

#endif
