#ifndef HARTSLAGSENSOR_H
#define HARTSLAGSENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

class HartslagSensor {
public:
    HartslagSensor();

    bool begin(TwoWire &wire = Wire);
    void update();

    float getBPM() const;
    int getAvgBPM() const;
    long getIR() const;

private:
    MAX30105 sensor;

    static const byte RATE_SIZE = 4;
    byte rates[RATE_SIZE];
    byte rateSpot = 0;

    long lastBeat = 0;
    float bpm = 0;
    int avgBPM = 0;
    long irValue = 0;
};

#endif
