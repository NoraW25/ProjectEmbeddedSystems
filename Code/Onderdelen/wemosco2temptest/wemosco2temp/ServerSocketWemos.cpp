#include "ServerSocketWemos.h"

ServerSocketWemos::ServerSocketWemos(int poort)
    : server(poort), buffer("") {}

/**
 * @brief Start de WiFi server en zet TCP no-delay
 */
void ServerSocketWemos::begin()
{
  server.begin();
  server.setNoDelay(true);
}

/**
 * @brief Controleer en accepteer eventueel een nieuwe client
 * @return true als er een client verbonden is
 */
bool ServerSocketWemos::heeftClient()
{
  if (!client.connected())
  {
    WiFiClient newClient = server.available();
    if (newClient)
    {
      client = newClient;
    }
  }
  return client.connected();
}

/**
 * @brief Lees inkomende bytes tot een newline en zet ze in de buffer
 * @return true als een volledige regel ontvangen is
 */
bool ServerSocketWemos::heeftOntvangen()
{
  if (!heeftClient())
  {
    return false;
  }

  while (client.available())
  {
    char c = client.read();
    if (c == '\n')
    {
      return true;
    }
    buffer += c;
  }
  return false;
}

/**
 * @brief Haal de ontvangen buffer op en maak deze leeg
 * @return Ontvangen bericht
 */
String ServerSocketWemos::ontvangst()
{
  String msg = buffer;
  buffer = "";
  return msg;
}

/**
 * @brief Verstuur een bericht naar de verbonden client
 * @param msg Bericht
 */
void ServerSocketWemos::versturen(const String &msg)
{
  if (heeftClient())
  {
    client.print(msg);
  }
}
