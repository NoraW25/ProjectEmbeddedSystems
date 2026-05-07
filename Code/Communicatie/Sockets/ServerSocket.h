
#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include "InterfaceSocket.h"

class ServerSocket: public InterfaceSocket {
public:
    void versturen(std::string) override;  
    std::string ontvangst() override;
    bool kanVersturen() override;
    bool heeftOntvangen() override;

    static ServerSocket* instantie();
    ~ServerSocket();

private:
    ServerSocket();
    
    int poort;
    char buffer[1024];

    int server_fd;
    int actieve_socket;
    struct sockaddr_in adres;

    static ServerSocket* pointerInstantie;
};

#endif