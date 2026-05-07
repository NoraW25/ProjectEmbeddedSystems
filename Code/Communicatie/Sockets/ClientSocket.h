#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include "InterfaceSocket.h"

class ClientSocket: public InterfaceSocket {
public:
    void versturen(std::string) override;  
    std::string ontvangst() override;
    bool kanVersturen() override;
    bool heeftOntvangen() override;
    
    bool heeftVerbinding();

    ClientSocket();
    ~ClientSocket();

private:
    int poort;
    char buffer[1024];
    int status;

    int client_fd;
    struct sockaddr_in server_adres;
    std::string server_ip; // Vul voor het ip-adres het adres in wat je via dhcp voor de server hebt gekregen

};

#endif