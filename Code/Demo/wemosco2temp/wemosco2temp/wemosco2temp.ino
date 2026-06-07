#include <Wire.h>
//#include "Adafruit_SGP30.h"
#include "SensorSHT31.h"
#include "ServerSocketWemos.h"
#include "MessageTranslator.h"
#include "LampPWM.h"

//Adafruit_SGP30 sgp;
SensorSHT31 sensortemp;
ServerSocketWemos server(8080);
MessageTranslator* translator = MessageTranslator::instance();
LampPWM lamp(14);

const char* ssid = "NSELab";
const char* password = "NSELabWiFi";

unsigned long last_time_send = 0;
const unsigned long send_interval = 1000; 

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
  // if (!sgp.begin()) {
  //   Serial.println("SGP30 niet gevonden!");
  //   while (1)
  //     ;
  // }
  //Serial.println("SGP30 gestart");


  if (!sensortemp.begin()) {
    while (1)
      Serial.println("tempsensor niet gevonden");
  }
}

void loop() {
  
  
  // if (sgp.IAQmeasure()) {
  //   Serial.print("eCO2: ");
  //   Serial.print(sgp.eCO2);
  //   Serial.print(" ppm, TVOC: ");
  //   Serial.print(sgp.TVOC);
  //   Serial.println(" ppb");
  //   //co2+TVOC versturen
  //   String canidco2 = "610";
  //   String data1 = String(sgp.eCO2);
  //   String data2 = String(sgp.TVOC);
    
  //   //server.versturen(canidco2 + " " + data1 + " " + data2);
  // }

  unsigned long time_now = millis();

  if (time_now - last_time_send >= send_interval) {
      std::vector<uint8_t> data_to_send_temperature;
      std::vector<uint8_t> data_to_send_humidity;
      last_time_send = time_now;
      Serial.println(WiFi.localIP());
      // hier komt jouw verstuur-code
      sensortemp.run();
      //temp + luchtvochtigheid versturen
      sensortemp.update();
      //String canidtemp = "620";
      //String data1 = String();
      //String data2 = String();
      double temperature = sensortemp.getTemperature();
      double humidity = sensortemp.getHumidity();
      uint64_t raw_temperature;
      uint64_t raw_humidity;
      std::memcpy(&raw_temperature, &temperature, sizeof(double));
      std::memcpy(&raw_humidity, &humidity, sizeof(double));

      // LSB eerst
      for (int i = 0; i < 8; i++) {
          data_to_send_temperature.push_back((raw_temperature >> (8 * i)) & 0xFF);
          data_to_send_humidity.push_back((raw_humidity >> (8 * i)) & 0xFF);
      }

      //server.versturen(canidtemp + " " + data1 + " " + data2);
      String message_temperature = String(translator->translate(610, data_to_send_temperature).c_str()); // Deze addressen moeten hetzelfde zijn als op de RPi in het klimaatproces, 610 voor temperatuursensor1
      String message_humidity = String(translator->translate(620, data_to_send_humidity).c_str());
      
      Serial.print("Temperatuur: ");
      Serial.println(message_temperature);
      Serial.print("Luchtvochtigheid: ");
      Serial.println(message_humidity);
      server.versturen(message_temperature);
      server.versturen(message_humidity);
  }

  if(server.heeftOntvangen()){
    String received_message = server.ontvangst();
    int address = 0;
    std::vector<uint8_t> data;
    translator->translate(&address, &data, std::string(received_message.c_str()));


    if (address == 640) {
      int value = 0;
      for (size_t i = 0; i < data.size(); i++) {
          value |= (uint32_t)data[i] << (8 * i);
      }

      Serial.println("Value: " + value);

      if (value > 0){
        lamp.turnOn(value);
        Serial.println("Dutycycle veranderd");
      }else {
        lamp.turnOff();
        Serial.println("Ven uitgezet");
      }
           
    }
  }
}