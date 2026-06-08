#include <Arduino.h>
#include <TM1637Display.h>
#include "LedControl.h"
#include "binary.h"
#include "LampPWM.h"
#include "ServerSocketWemos.h"
#include "MessageTranslator.h"
#include <Wire.h>
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
#define lamp7 A0  //TX
#define lamp8 A0  //RX
#define lamp9 16

const char* ssid = "NSELab";
const char* password = "NSELabWiFi";
ServerSocketWemos server(8080);
MessageTranslator* translator = MessageTranslator::instance();


//lampen
int lamps[9] = {
  lamp1, lamp2, lamp3, lamp4, lamp5,
  lamp6, lamp7, lamp8, lamp9
};
unsigned long delaytime = 1000;
TM1637Display segdisplay(TM_CLK, TM_DIO);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  server.begin();

  segdisplay.setBrightness(0x0f);
  for (int i = 0; i < 9; i++) {
    pinMode(lamps[i], OUTPUT);
    digitalWrite(lamps[i], LOW);
  }
}


void loop() {
  Serial.println(WiFi.localIP());
  if (server.heeftOntvangen()) {

    //Serial.println("Hello in ontvangen");
    String received_message = server.ontvangst();
    int address = 0;
    std::vector<uint8_t> data;
    translator->translate(&address, &data, std::string(received_message.c_str()));


    if (address == 720) {
      int value = 0;
      for (size_t i = 0; i < data.size(); i++) {
        value |= (uint32_t)data[i] << (8 * i);
      }
      if (value < 10000) {
        segdisplay.showNumberDec(value, false);
      } else {
        segdisplay.showNumberDec(0, true);
      }
    }
    if (address == 720) {
      int value = 0;
      for (size_t i = 0; i < data.size(); i++) {
        value |= (uint32_t)data[i] << (8 * i);
      }

      if (!(value > 0)) {
        for (int i = 0; i < value; i++) {
          digitalWrite(lamps[i], HIGH);
        }
      }
    }
  }
}
