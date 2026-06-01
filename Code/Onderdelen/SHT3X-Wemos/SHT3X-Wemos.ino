#include <Arduino.h>
#include "SensorSHT31.h"

SensorSHT31 sensor;

void setup()
{
    Serial.begin(115200);

    if (!sensor.begin())
    {
        while (1)
        {
            delay(1);
        }
    }
}

void loop()
{
    sensor.run();
    delay(1000);
}