#ifndef HARTSLAGSENSOR_H
#define HARTSLAGSENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

/**
 * @file HartslagSensor.h
 * @brief Interface voor de hartslagsensor (MAX30105) wrapper
 */

/**
 * @class HartslagSensor
 * @brief Hanteert de MAX30105 sensor en berekent hartslagwaarden
 */
class HartslagSensor
{
public:
    /**
     * @brief Constructor
     */
    HartslagSensor();

    /**
     * @brief Initialiseert de sensor
     * @param wire I2C bus (standaard `Wire`)
     * @return true als initialisatie gelukt is
     */
    bool begin(TwoWire &wire = Wire);

    /**
     * @brief Lees nieuwe waarden van de sensor en update interne staten
     */
    void update();

    /**
     * @brief Retourneer de laatst berekende BPM (float)
     * @return BPM
     */
    float getBPM() const;

    /**
     * @brief Retourneer het gemiddelde BPM over de buffer
     * @return gemiddelde BPM
     */
    int getAvgBPM() const;

    /**
     * @brief Retourneer de laatst gelezen IR-waarde
     * @return IR waarde
     */
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
