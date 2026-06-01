#pragma once
#include <ESP8266WiFi.h>

class ServerSocketWemos {
public:
    ServerSocketWemos(int poort);

    void begin();
    bool heeftClient();
    bool heeftOntvangen();
    String ontvangst();
    void versturen(const String& msg);

private:
    WiFiServer server;
    WiFiClient client;
    String buffer;
};