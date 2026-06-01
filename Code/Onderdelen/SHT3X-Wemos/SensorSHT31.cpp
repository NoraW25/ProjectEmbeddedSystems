#include "SensorSHT31.h"

SensorSHT31::SensorSHT31()
{
    temperature = 0.0;
    humidity = 0.0;
}

bool SensorSHT31::begin(uint8_t address)
{
    Wire.begin(D2, D1);

    if (!sht31.begin(address))
    {
        Serial.println("Sensor not found!");
        return false;
    }

    Serial.println("SHT31 sensor gevonden!");
    return true;
}

void SensorSHT31::update()
{
    temperature = sht31.readTemperature();
    humidity = sht31.readHumidity();
}

float SensorSHT31::getTemperature()
{
    return temperature;
}

float SensorSHT31::getHumidity()
{
    return humidity;
}

void SensorSHT31::printData()
{
    Serial.print("Temperatuur: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Luchtvochtigheid: ");
    Serial.print(humidity);
    Serial.println(" %");
}

void SensorSHT31::run()
{
    update();
    printData();
}

