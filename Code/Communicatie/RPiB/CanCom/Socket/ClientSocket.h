
/*!
 * @file ClientSocket.h
 * @class ClientSocket
 * @brief Maakt een client socket aan, kan daardoor berichten ontvangen en versturen.
 * Met een object van deze klasse, wordt er eerst een connectie tussen de sockets opgezet, die non blocking werkt.
 * Daarna kunnen er berichten worden ontvangen en verzonden.
 */

#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include "Socket.h"
#include "../Translator/MessageTranslator.h"
#include <cstdint>

class ClientSocket: public Socket {
public:
    virtual void send(int, std::vector<uint8_t>) override;  
    virtual bool received(int*, std::vector<uint8_t>*) override;
    virtual bool canSend() override;
    

    /*!
    * @brief Bekijkt of er verbinding is tussen de client socket en de server socket.
    *
    * Deze functie bekijkt met behulp van de status van het object of er verbinding is met de server of niet.
    * Als er geen verbinding is zal er geprobeerd worden om verbinding te maken.
    * 
    * @return Boolean - Terugkoppeling op of er wel of geen verbinding is met de server. Bij verbinding: true.
    */
    bool hasConnection();

    /*!
    * @brief Maakt de ClientSocket aan.
    *
    * Deze constructor zorgt ervoor dat de basis van de ClientSocket er in ieder geval is. 
    * Door de nonblocking structuur van de code, kunnen er andere werkzaamheden tegelijk verlopen, 
    * maar zorgt er ook voor dat de constructor de kans loopt dat de verbinding nog niet volledig is opgezet als de constructor zijn werk heeft gedaan.
    * 
    * @param string - IP-adres waarmee verbinding gemaakt moet worden.
    */
    ClientSocket(std::string);    
    ClientSocket(std::string, int, bool);

    /*!
    * @brief Sluit de ClientSocket.
    *
    * Als de ClientSocket nog actief is, zal deze worden afgesloten.
    */
    ~ClientSocket();

private:
    int port;
    char buffer[1024];
    int status; // -1 = Connectie gefaald; 0 = Verbinding; 1 = Bezig met verbinden.
    
    bool is_wemos;

    void tcpStartup();
    bool hasReceived();

    int client_fd;
    struct sockaddr_in server_address;
    std::string server_ip; // Vul voor het ip-adres het adres in wat je via dhcp voor de server hebt gekregen

    MessageTranslator* translator;
};

#endif