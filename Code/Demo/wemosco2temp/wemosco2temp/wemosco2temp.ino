#include <Wire.h>
#include "Adafruit_SGP30.h"
#include "SensorSHT31.h"
#include "ServerSocketWemos.h"

Adafruit_SGP30 sgp;
SensorSHT31 sensortemp;
ServerSocketWemos server(8080);

const char* ssid = "NSELab";
const char* password = "NSELabWiFi";

void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nVerbonden!");
  Serial.println(WiFi.localIP());
  server.begin();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
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
  Serial.println(WiFi.localIP());
  if (sgp.IAQmeasure()) {
    Serial.print("eCO2: ");
    Serial.print(sgp.eCO2);
    Serial.print(" ppm, TVOC: ");
    Serial.print(sgp.TVOC);
    Serial.println(" ppb");
    //co2+TVOC versturen
    String canidco2 = "610";
    String data1 = String(sgp.eCO2);
    String data2 = String(sgp.TVOC);
    server.versturen(canidco2 + " " + data1 + " " + data2);
  }
  sensortemp.run();
  //temp + luchtvochtigheid versturen
  sensortemp.update();
  String canidtemp = "620";
  String data1 = String(sensortemp.getTemperature());
  String data2 = String(sensortemp.getHumidity());
  server.versturen(canidtemp + " " + data1 + " " + data2);
  delay(1000);
}