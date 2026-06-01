#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

#include "ServerSocketWemos.h"
#include "HartslagSensor.h"

const char* ssid = "NSELab";
const char* password = "NSELabWiFi";

ServerSocketWemos server(8080);
HartslagSensor hartslag;

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
    while (1);
  }
}

void loop() {
  hartslag.update();

  long ir = hartslag.getIR();
  float bpm = hartslag.getBPM();
  int avg = hartslag.getAvgBPM();

  Serial.printf("IR=%ld, BPM=%.1f, Avg BPM=%d\n", ir, bpm, avg);

  if (millis() - lastSend >= 1000) {
    lastSend = millis();

    if (server.heeftClient()) {
      server.versturen(String(avg));
    }
  }
}
