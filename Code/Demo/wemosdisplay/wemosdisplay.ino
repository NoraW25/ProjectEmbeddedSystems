#include <Arduino.h>
/**
 * @file wemosdisplay.ino
 * @brief Hoofdsketch voor de Wemos: initialiseert WiFi, server en display.
 *
 * Deze sketch start een `ServerSocketWemos`, gebruikt `MessageTranslator`
 * om binnenkomende berichten te parsen en bestuurt lampen en een 7-seg
 * display.
 */
#include <TM1637Display.h>
#include "LedControl.h"
#include "binary.h"
#include "LampPWM.h"
#include "ServerSocketWemos.h"
#include "MessageTranslator.h"
#include <Wire.h>
#include "wifi.h"
// TM1637
#define TM_CLK D1
#define TM_DIO D2
//https://cyaninfinite.com/getting-started-with-the-wemos-d1-esp8266-wifi-board/#Pinouts
#define lamp1 0
#define lamp2 2
#define lamp3 14
#define lamp4 12
#define lamp5 13
#define lamp6 15
#define lamp7 TX
#define lamp8 RX
#define lamp9 16


ServerSocketWemos server(8080);
MessageTranslator* translator = MessageTranslator::instance();


//lampen
int lamps[9] = {
  lamp9, lamp8, lamp7, lamp6, lamp5,
  lamp4, lamp3, lamp2, lamp1
};
unsigned long delaytime = 1000;
TM1637Display segdisplay(TM_CLK, TM_DIO);

void setup() {
  //Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  server.begin();

  segdisplay.setBrightness(0x0f);
  for (int i = 0; i < 9; i++) {
    pinMode(lamps[i], OUTPUT);
    digitalWrite(lamps[i], LOW);
  }
}


void loop() {
  //Serial.println(WiFi.localIP());
  if (server.heeftOntvangen()) {

    //Serial.println("Hello in ontvangen");
    String received_message = server.ontvangst();
    int address = 0;
    std::vector<uint8_t> data;
    translator->translate(&address, &data, std::string(received_message.c_str()));


    if (address == 710) {
      uint64_t raw = 0;

      for (size_t i = 0; i < data.size() && i < sizeof(raw); i++) {
          raw |= (uint64_t)data[i] << (8 * i);
      }

      double value;
      std::memcpy(&value, &raw, sizeof(double));

      //Serial.println(value);
        if (value < 10000) {
          segdisplay.showNumberDec((int)value, false);
        } else {
          segdisplay.showNumberDec(0, true);
        }
    }

    if (address == 720) {
      int value = 0;
      for (size_t i = 0; i < data.size(); i++) {
        value |= (uint32_t)data[i] << (8 * i);
      }

      if (value >= 0) {
        for (int i = 0; i < value; i++) {
          digitalWrite(lamps[i], HIGH);
        }
        for (int i = value; i < 9; i++){
          digitalWrite(lamps[i], LOW);
        }

      }
    }
  }
}
