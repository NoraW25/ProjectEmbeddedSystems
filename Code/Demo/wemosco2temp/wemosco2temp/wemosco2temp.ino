#include <Wire.h>
#include "Adafruit_SGP30.h"
#include "SensorSHT31.h"

Adafruit_SGP30 sgp;
SensorSHT31 sensortemp;



void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1);
  if (!sgp.begin()) {
    Serial.println("SGP30 niet gevonden!");
    while (1)
      ;
  }
  Serial.println("SGP30 gestart");


  if (!sensortemp.begin()) {
    while (1)
    Serial.println("tempsensor niet gevonden");
  }
}

  void loop() {
    if (sgp.IAQmeasure()) {
      Serial.print("eCO2: ");
      Serial.print(sgp.eCO2);
      Serial.print(" ppm, TVOC: ");
      Serial.print(sgp.TVOC);
      Serial.println(" ppb");
      delay(500);
      sensortemp.run();
    }
  }