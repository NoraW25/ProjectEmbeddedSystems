
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
#include <ctime>

class ClientSocket: public Socket {
public:
    virtual void send(int, std::vector<uint8_t>) override;  
    virtual bool received(int*, std::vector<uint8_t>*) override;
    virtual bool canSend() override;

    /*!
    * @brief Maakt de ClientSocket aan.
    *
    * Deze constructor zorgt ervoor dat de basis van de ClientSocket er in ieder geval is. 
    * Door de nonblocking structuur van de code, kunnen er andere werkzaamheden snel na elkaar verlopen, 
    * maar dat zorgt er ook voor dat de constructor de kans loopt dat de verbinding nog niet volledig is opgezet 
    * als de constructor zijn werk heeft gedaan.
    * 
    * Standaard wordt er met portnummer 8080 verbinding geprobeert te maken.
    * Daarnaast wordt er standaard vanuitgegaan dat de client GEEN Wemos is.
    * 
    * @param string - IP-adres waarmee verbinding gemaakt moet worden.
    */
    ClientSocket(std::string);    

    /*!
    * @brief Maakt de ClientSocket aan.
    *
    * Deze constructor zorgt ervoor dat de basis van de ClientSocket er in ieder geval is. 
    * Door de nonblocking structuur van de code, kunnen er andere werkzaamheden snel na elkaar verlopen, 
    * maar dat zorgt er ook voor dat de constructor de kans loopt dat de verbinding nog niet volledig is opgezet 
    * als de constructor zijn werk heeft gedaan.
    *     
    * Standaard wordt er met portnummer 8080 verbinding geprobeert te maken.
    * 
    * @param string - IP-adres waarmee verbinding gemaakt moet worden.
    * @param bool - Geeft aan of de clientsocket verbinding maakt met een Wemos server. True voor met de Wemos, False voor geen Wemos.
    */
    ClientSocket(std::string, bool);

    /*!
    * @brief Maakt de ClientSocket aan.
    *
    * Deze constructor zorgt ervoor dat de basis van de ClientSocket er in ieder geval is. 
    * Door de nonblocking structuur van de code, kunnen er andere werkzaamheden snel na elkaar verlopen, 
    * maar dat zorgt er ook voor dat de constructor de kans loopt dat de verbinding nog niet volledig is opgezet 
    * als de constructor zijn werk heeft gedaan.
    * 
    * @param string - IP-adres waarmee verbinding gemaakt moet worden.
    * @param int - Poortnummer waarmee verbinding gemaakt moet worden.
    * @param bool - Geeft aan of de clientsocket verbinding maakt met een Wemos server. True voor met de Wemos, False voor geen Wemos.
    */
    ClientSocket(std::string, int, bool);

    /*!
    * @brief Sluit de ClientSocket.
    *
    * Als de ClientSocket nog actief is, zal deze worden afgesloten.
    */
    virtual ~ClientSocket();

private:
    int port;
    char buffer[1024];
    int status; // -1 = Connectie gefaald; 0 = Verbinding; 1 = Bezig met verbinden.
    
    bool is_wemos;
    std::time_t last_reconnection_time;

    /*!
    * @brief Zet de basics voor de socket.
    * 
    * Deze methode moet standaard worden aangeroepen in een contructor.
    * Dit zet de waardes in het object goed en probeert vervolgens een connectie te maken met de server.
    * 
    */
    void tcpStartup();

    /*!
    * @brief Probeert opnieuw connectie te maken met de server.
    * 
    * Deze methode sluit de socket als deze nog niet gesloten is, en bouwt het met de tcpStartup() socket weer op.
    * Vervolgens wordt er in tcpStartup ook weer geprobeert om een verbinding met de server te maken.
    * 
    * Er zit minimaal 2 seconden tussen de verschillende pogingen om te connecten.
    * 
    */
    void reconnect();



    int client_fd;
    struct sockaddr_in server_address;
    std::string server_ip; // Vul voor het ip-adres het adres in wat je via dhcp voor de server hebt gekregen

    MessageTranslator* translator;
};

#endif