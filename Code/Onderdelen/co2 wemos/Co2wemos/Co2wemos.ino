#include <Wire.h>
#include "Adafruit_SGP30.h"

Adafruit_SGP30 sgp;



void setup() {
  Serial.begin(115200);

  // I2C starten op D2 (SDA) en D1 (SCL)
  Wire.begin(D2, D1);

  // --- SGP30 ---
  if (!sgp.begin()) {
    Serial.println("SGP30 niet gevonden!");
    while (1);
  }
  Serial.println("SGP30 gestart");
}

void loop() {

  // --- SGP30 meting ---
  if (sgp.IAQmeasure()) {
    Serial.print("eCO2: ");
    Serial.print(sgp.eCO2);
    Serial.print(" ppm, TVOC: ");
    Serial.print(sgp.TVOC);
    Serial.println(" ppb");
  delay(500);
}
}