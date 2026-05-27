#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "ServerSocketWemos.h"
#include "SensorSHT31.h"

const char* ssid = "NSELab";
const char* password = "NSELabWiFi";

ServerSocketWemos server(8080);
SensorSHT31 sensor;

void setup()
{
    Serial.begin(115200);

Serial.println("Start WiFi verbinding...");

WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);

int poging = 0;

while (WiFi.status() != WL_CONNECTED && poging < 20)
{
    delay(500);
    Serial.print(".");
    poging++;
}

if (WiFi.status() == WL_CONNECTED)
{
    Serial.println("\n✅ Verbonden!");
    Serial.print("IP adres: ");
    Serial.println(WiFi.localIP());
}
else
{
    Serial.println("\n❌ WiFi MISLUKT!");
}

    server.begin();


    if (!sensor.begin())
    {
        while (1)
        {
            delay(1);
        }
    }
}

void loop()
{
    sensor.run();

    
    String data = sensor.getDataString();

    if (server.heeftClient()) 
    {
        server.versturen(data);
    }


    if (server.heeftClient()) {

        if (server.heeftOntvangen()) {
            String msg = server.ontvangst();
            Serial.print("Ontvangen: ");
            Serial.println(msg);

            server.versturen("Hallo vanaf de Wemos!");
        }
    }

    delay(1000);
}