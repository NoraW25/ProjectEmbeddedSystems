#include "HartslagSensor.h"

HartslagSensor::HartslagSensor() {}

bool HartslagSensor::begin(TwoWire &wire) {
    if (!sensor.begin(wire, I2C_SPEED_FAST)) {
        return false;
    }

    sensor.setup();
    sensor.setPulseAmplitudeRed(0x0A);
    sensor.setPulseAmplitudeGreen(0);

    return true;
}

void HartslagSensor::update() {
    irValue = sensor.getIR();

    if (checkForBeat(irValue)) {
        long delta = millis() - lastBeat;
        lastBeat = millis();

        bpm = 60 / (delta / 1000.0);

        if (bpm > 20 && bpm < 255) {
            rates[rateSpot++] = (byte)bpm;
            rateSpot %= RATE_SIZE;

            avgBPM = 0;
            for (byte i = 0; i < RATE_SIZE; i++) {
                avgBPM += rates[i];
            }
            avgBPM /= RATE_SIZE;
        }
    }
}

float HartslagSensor::getBPM() const {
    return bpm;
}

int HartslagSensor::getAvgBPM() const {
    return avgBPM;
}

long HartslagSensor::getIR() const {
    return irValue;
}
