#include <Arduino.h>
#include <RotaryEncoder.h>
#include <Wire.h>
#include <string.h>
#include "ServerSocketWemos.h"
#include "MessageTranslator.h"
#include "wifi.h"

#define PIN_IN1 D5
#define PIN_IN2 D6
RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);
ServerSocketWemos server(8080);
MessageTranslator* translator = MessageTranslator::instance();
bool rondje = 0;
int stappen = 0;
long lastPos = 0;   
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  server.begin();
}


// Read the current position of the encoder and print out when changed.
void loop() {
  encoder.tick();
  long newPos = encoder.getPosition();

  if (newPos != lastPos) {
    int delta = newPos - lastPos;   
    stappen += delta;              
    lastPos = newPos;

    Serial.print("pos:");
    Serial.print(newPos);
    Serial.print(" stappen:");
    Serial.println(stappen);
  }
  if (stappen >= 35 || stappen <= -35) {
    stappen = 0;  

    std::vector<uint8_t> data;
    data.push_back(1);
    String msg = String(translator->translate(910, data).c_str());
    server.versturen(msg);

    Serial.println("RONDJE!");
  }
}