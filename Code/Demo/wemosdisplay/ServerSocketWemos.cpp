#include "ServerSocketWemos.h"

ServerSocketWemos::ServerSocketWemos(int poort)
  : server(poort), buffer("") {}

void ServerSocketWemos::begin() {
  server.begin();
  server.setNoDelay(true);
}

bool ServerSocketWemos::heeftClient() {
  if (!client.connected()) {
    WiFiClient newClient = server.available();
    if (newClient) {
      client = newClient;
    }
  }
  return client.connected();
}

bool ServerSocketWemos::heeftOntvangen() {
  if (!heeftClient()) {
    return false;
  }

  while (client.available()) {
    char c = client.read();
    if (c == '\n') {
      return true;
    }
    buffer += c;
  }
  return false;
}

String ServerSocketWemos::ontvangst() {
  String msg = buffer;
  buffer = "";
  return msg;
}

void ServerSocketWemos::versturen(const String& msg) {
  if (heeftClient()) {
    client.print(msg);
  }
}
