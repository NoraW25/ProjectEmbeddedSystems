
/*!
 * @file ServerSocket.h
 * @class ServerSocket
 * @brief Maakt een server socket aan en kan daardoor berichten ontvangen en versturen.
 * Met een object van deze klasse wordt er eerst een connectie tussen de sockets opgezet, die non blocking werkt.
 * Daarna kunnen er berichten worden ontvangen en verzonden tussen de sockets.
 * 
 * Deze klasse is een singleton, er kan dus op een apparataat maar 1 serversocket worden aangemaakt. 
 * Als er meerdere clientsockets zijn, zullen die dus via dezelfde serversocket geholpen moeten worden.
 */

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include "InterfaceSocket.h"

class ServerSocket: public InterfaceSocket {
public:
    void send(std::string) override;  
    std::string received() override;
    bool canSend() override;
    bool hasReceived() override;

    /*!
     * @brief Haalt het adres van de serversocket op.
     *
     * Deze statische methode zorgt ervoor dat er maar 1 instantie bestaat van de serversocket op een apparaat.
     * Eerst wordt er bekeken of er al een instantie bestaat van ServerSocket.
     * - Als deze bestaat wordt meteen het adres teruggegeven.
     * - Als deze niet bestaat wordt er een ServerSocket object aangemaakt. Daarvan wordt vervolgens het adres teruggegeven.
     * 
     * @return ServerSocket* - Een pointer naar het enige object van ServerSocket.
     */
    static ServerSocket* instance();

    /*!
     * @brief Sluit luisterende en actieve sockets.
     *
     * Indien er luisterende en/of actieve sockets aangewezig zijn worden deze afgesloten.
     */
    ~ServerSocket();

private:
    /*!
     * @brief Maakt de ServerSocket aan.
     *
     * Deze constructor zorgt ervoor dat de basis van de ClientSocket er in ieder geval is. 
     * Daarnaast wordt via de static variable aangegeven dat er een instantie van de klasse ServerSocket is, door het adres van het object toe te kennen.
     */
    ServerSocket();
    
    int port;
    char buffer[1024];

    int server_fd;
    int active_socket;
    struct sockaddr_in address;

    static ServerSocket* pointerInstance;
};

#endif