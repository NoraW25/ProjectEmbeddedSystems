#include <Wire.h>
#include "Adafruit_SGP30.h"
#include "SensorSHT31.h"
#include "ServerSocketWemos.h"
#include "MessageTranslator.h"
#include "LampPWM.h"
#include "wifi.h"


Adafruit_SGP30 sgp;
SensorSHT31 sensortemp;
ServerSocketWemos server(8080);
MessageTranslator* translator = MessageTranslator::instance();
LampPWM lamp(14);


enum SendState {
  SEND_CO2,
  SEND_TEMP,
  SEND_HUM
};

SendState sendState = SEND_CO2;
unsigned long last_time_send = 0;
const unsigned long send_interval = 8000;

void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  server.begin();
  Serial.println("\nVerbonden!");

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
  unsigned long time_now = millis();

if (time_now - last_time_send >= send_interval) {

  sensortemp.run();
  sensortemp.update();
  sgp.IAQmeasure();

  double temperature = sensortemp.getTemperature();
  double humidity = sensortemp.getHumidity();
  double co2 = sgp.eCO2;

  std::vector<uint8_t> data;

  if (sendState == SEND_CO2) {

    uint64_t raw_co2;
    std::memcpy(&raw_co2, &co2, sizeof(double));

    for (int i = 0; i < 8; i++) {
      data.push_back((raw_co2 >> (8 * i)) & 0xFF);
    }

    String msg = String(translator->translate(620, data).c_str());
    server.versturen(msg);

    sendState = SEND_TEMP;
    Serial.println("CO2 verstuurd");
  }


  else if (sendState == SEND_TEMP) {

    uint64_t raw_temperature;
    std::memcpy(&raw_temperature, &temperature, sizeof(double));

    for (int i = 0; i < 8; i++) {
      data.push_back((raw_temperature >> (8 * i)) & 0xFF);
    }

    String msg = String(translator->translate(610, data).c_str());
    server.versturen(msg);

    sendState = SEND_HUM;
    Serial.println("Temperatuur verstuurd");
  }


  else if (sendState == SEND_HUM) {

    uint64_t raw_humidity;
    std::memcpy(&raw_humidity, &humidity, sizeof(double));

    for (int i = 0; i < 8; i++) {
      data.push_back((raw_humidity >> (8 * i)) & 0xFF);
    }

    String msg = String(translator->translate(630, data).c_str());
    server.versturen(msg);

    sendState = SEND_CO2;
    Serial.println("Humidity verstuurd");

    // ronde volledig afgerond → timer reset
    last_time_send = time_now;
  }
}
/*
  if (time_now - last_time_send >= send_interval) {
    
    if (sgp.IAQmeasure()) {
      Serial.print("eCO2: ");
      Serial.print(sgp.eCO2);
      Serial.print(" ppm, TVOC: ");
      Serial.print(sgp.TVOC);
      Serial.println(" ppb");
      // co2 + TVOC versturen String canidco2 = "610";
      String data1 = "ID:620;DATA:" + String(sgp.eCO2);
      server.versturen(data1);
    }
    
    std::vector<uint8_t> data_to_send_co2; 
    std::vector<uint8_t> data_to_send_temperature;
    std::vector<uint8_t> data_to_send_humidity;
    last_time_send = time_now;
    Serial.println(WiFi.localIP());
    sensortemp.run();
    sensortemp.update();
    sgp.IAQmeasure();
    double temperature = sensortemp.getTemperature();
    double humidity = sensortemp.getHumidity();
    double co2 = sgp.eCO2;
      Serial.println("co2: ");
      Serial.println(sgp.eCO2);

    uint64_t raw_temperature;
    uint64_t raw_humidity;
    uint64_t raw_co2;
    std::memcpy(&raw_co2, &co2, sizeof(double));
    std::memcpy(&raw_temperature, &temperature, sizeof(double));
    std::memcpy(&raw_humidity, &humidity, sizeof(double));

    // LSB eerst
    for (int i = 0; i < 8; i++) {
      data_to_send_temperature.push_back((raw_temperature >> (8 * i)) & 0xFF);
      data_to_send_humidity.push_back((raw_humidity >> (8 * i)) & 0xFF);
      data_to_send_co2.push_back((raw_co2 >> (8 * i)) & 0xFF); 
    }
    String message_temperature = String(translator->translate(610, data_to_send_temperature).c_str());  // Deze addressen moeten hetzelfde zijn als op de RPi in het klimaatproces, 610 voor temperatuursensor1
    String message_humidity = String(translator->translate(630, data_to_send_humidity).c_str());
    String message_co2 = String(translator->translate(620, data_to_send_co2).c_str());


   // Serial.print("Temperatuur: ");
    //Serial.println(message_temperature);
    //Serial.print("Luchtvochtigheid: ");
    //Serial.println(message_humidity);
    //Serial.println("co2: ");
    //Serial.println(message_co2);
    server.versturen(message_co2);
    delay(100);
    server.versturen(message_temperature);
    delay(100);
    server.versturen(message_humidity);
    delay(100);
  }
  */

  if (server.heeftOntvangen()) {
    //Serial.println("Hello in ontvangen");
    String received_message = server.ontvangst();
    Serial.println(received_message);
    int address = 0;
    std::vector<uint8_t> data;
    translator->translate(&address, &data, std::string(received_message.c_str()));


    if (address == 640) {
      int value = 0;
      for (size_t i = 0; i < data.size(); i++) {
        value |= (uint32_t)data[i] << (8 * i);
      }

      Serial.println(value);

      if (value > 0) {
        lamp.turnOn(value);
        Serial.println("Dutycycle veranderd");
      } else {
        lamp.turnOff();

        Serial.println("Ven uitgezet");
      }
    }
  }
}