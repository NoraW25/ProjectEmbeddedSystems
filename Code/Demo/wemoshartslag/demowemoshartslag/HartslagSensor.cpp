#include "HartslagSensor.h"

/**
 * @brief Constructor
 */
HartslagSensor::HartslagSensor() {}

/**
 * @brief Initialiseert de sensor hardware
 * @param wire I2C bus om te gebruiken
 * @return true als de sensor succesvol is geïnitialiseerd
 */
bool HartslagSensor::begin(TwoWire &wire)
{
    if (!sensor.begin(wire, I2C_SPEED_FAST))
    {
        return false;
    }

    sensor.setup();
    sensor.setPulseAmplitudeRed(0x0A);
    sensor.setPulseAmplitudeGreen(0);

    return true;
}

/**
 * @brief Lees sensorwaarden en bereken BPM indien beat gedetecteerd
 */
void HartslagSensor::update()
{
    irValue = sensor.getIR();

    if (checkForBeat(irValue))
    {
        long delta = millis() - lastBeat;
        lastBeat = millis();

        bpm = 60 / (delta / 1000.0);

        if (bpm > 20 && bpm < 255)
        {
            rates[rateSpot++] = (byte)bpm;
            rateSpot %= RATE_SIZE;

            avgBPM = 0;
            for (byte i = 0; i < RATE_SIZE; i++)
            {
                avgBPM += rates[i];
            }
            avgBPM /= RATE_SIZE;
        }
    }
}

/**
 * @brief Verkrijg de laatst berekende BPM
 * @return BPM waarde als float
 */
float HartslagSensor::getBPM() const
{
    return bpm;
}

/**
 * @brief Verkrijg het gemiddelde BPM over de buffer
 * @return Gemiddelde BPM als int
 */
int HartslagSensor::getAvgBPM() const
{
    return avgBPM;
}

/**
 * @brief Verkrijg de laatst gelezen IR-waarde
 * @return IR waarde
 */
long HartslagSensor::getIR() const
{
    return irValue;
}
