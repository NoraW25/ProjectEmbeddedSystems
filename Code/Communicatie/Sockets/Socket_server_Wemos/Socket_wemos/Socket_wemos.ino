#include <ESP8266WiFi.h>
#include "ServerSocketWemos.h"

const char* ssid = "NSELab";
const char* password = "NSELabWiFi";

ServerSocketWemos server(8080);

void setup() {
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.print("Verbinden met WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nVerbonden!");
    Serial.print("IP adres: ");
    Serial.println(WiFi.localIP());

    server.begin();
}

void loop() {
    if (server.heeftClient()) {

        if (server.heeftOntvangen()) {
            String msg = server.ontvangst();
            Serial.print("Ontvangen: ");
            Serial.println(msg);

            server.versturen("Hallo vanaf de Wemos!");
        }
    }
}
