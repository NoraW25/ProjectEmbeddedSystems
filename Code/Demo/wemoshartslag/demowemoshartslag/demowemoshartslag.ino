#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

#include "ServerSocketWemos.h"
#include "HartslagSensor.h"
#include "MessageTranslator.h"
const char* ssid = "NSELab";
const char* password = "NSELabWiFi";

ServerSocketWemos server(8080);
HartslagSensor hartslag;
MessageTranslator* translator = MessageTranslator::instance();

unsigned long lastSend = 0;

void setup() {
  Serial.begin(115200);
  delay(200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nVerbonden!");
  Serial.println(WiFi.localIP());

  server.begin();

  if (!hartslag.begin(Wire)) {
    Serial.println("MAX30105 niet gevonden!");
    while (1)
      ;
  }
}

void loop() {
  hartslag.update();
  long ir = hartslag.getIR();
  float bpm = hartslag.getBPM();
  double avg = hartslag.getAvgBPM();


  if (millis() - lastSend >= 1000) {
    Serial.printf("IR=%ld, BPM=%.1f, Avg BPM=%.1f\n", ir, bpm, avg);
    Serial.println(WiFi.localIP());
    lastSend = millis();
    std::vector<uint8_t> data_to_send_bpm_avg;
    uint64_t raw_bpm_avg;
    std::memcpy(&raw_bpm_avg, &avg, sizeof(double));
    for (int i = 0; i < 8; i++) {
      data_to_send_bpm_avg.push_back((raw_bpm_avg >> (8 * i)) & 0xFF);
    }
    String message_avg_bpm = String(translator->translate(810, data_to_send_bpm_avg).c_str());
    server.versturen(message_avg_bpm);
  }
}
