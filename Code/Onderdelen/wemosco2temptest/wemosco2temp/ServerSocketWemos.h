#pragma once
#include <ESP8266WiFi.h>

/**
 * @file ServerSocketWemos.h
 * @brief Eenvoudige WiFi server wrapper voor Wemos (ESP8266)
 */

/**
 * @class ServerSocketWemos
 * @brief Beheert een `WiFiServer` en client-communicatie
 */
class ServerSocketWemos
{
public:
    /**
     * @brief Maak een server die luistert op een poort
     * @param poort Luisterpoort
     */
    ServerSocketWemos(int poort);

    /**
     * @brief Start de server
     */
    void begin();

    /**
     * @brief Controleer of er een client verbonden is (en accepteer nieuwe)
     * @return true als er een client verbonden is
     */
    bool heeftClient();

    /**
     * @brief Controleer of er een volledige regel (message) ontvangen is
     * @return true als er een ontvangen bericht klaarstaat
     */
    bool heeftOntvangen();

    /**
     * @brief Haal de ontvangen berichtbuffer op en maak deze leeg
     * @return Ontvangen bericht als `String`
     */
    String ontvangst();

    /**
     * @brief Verstuur een bericht naar de verbonden client
     * @param msg Bericht om te versturen
     */
    void versturen(const String &msg);

private:
    WiFiServer server;
    WiFiClient client;
    String buffer;
};
