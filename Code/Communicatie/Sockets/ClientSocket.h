/*!
 * @file ClientSocket.h
 * @class ClientSocket
 * @brief Maakt een client socket aan, kan daardoor berichten ontvangen en versturen.
 * Met een object van deze klasse, wordt er eerst een connectie tussen de sockets opgezet, die non blocking werkt.
 * Daarna kunnen er berichten worden ontvangen en verzonden.
 */

#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include "InterfaceSocket.h"

class ClientSocket: public InterfaceSocket {
public:
    void versturen(std::string) override;  
    std::string ontvangst() override;
    bool kanVersturen() override;
    bool heeftOntvangen() override;
    

    /*!
    * @brief Bekijkt of er verbinding is tussen de client socket en de server socket.
    *
    * Deze functie bekijkt met behulp van de status van het object of er verbinding is met de server of niet.
    * Als er geen verbinding is zal er geprobeerd worden om verbinding te maken.
    * 
    * @return Boolean: terugkoppeling op of er wel of geen verbinding is met de server. Bij verbinding: true.
    */
    bool heeftVerbinding();

    /*!
    * @brief Maakt de ClientSocket aan.
    *
    * Deze constructor zorgt ervoor dat de basis van de ClientSocket er in ieder geval is. 
    * Door de nonblocking structuur van de code, kunnen er andere werkzaamheden tegelijk verlopen, 
    * maar zorgt er ook voor dat de constructor de kans loopt dat de verbinding nog niet volledig is opgezet als de constructor zijn werk heeft gedaan.
    */
    ClientSocket();

    /*!
    * @brief Sluit de ClientSocket.
    *
    * Als de ClientSocket nog actief is, zal deze worden afgesloten.
    */
    ~ClientSocket();

private:
    int poort;
    char buffer[1024];
    int status; // -1 = Connectie gefaald; 0 = Verbinding; 1 = Bezig met verbinden.

    int client_fd;
    struct sockaddr_in server_adres;
    std::string server_ip; // Vul voor het ip-adres het adres in wat je via dhcp voor de server hebt gekregen

};

#endif