
/*!
 * @file InterfaceSocket.h
 * @class InterfaceSocket
 * @brief Een interface voor het maken van Sockets. 
 */

#ifndef INTERFACESOCKET_H
#define INTERFACESOCKET_H

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>


class InterfaceSocket{
public:
    /*!
     * @brief Verstuurd een bericht.
     *
     * Deze methode bekijkt of er verstuurd kan worden met kanVersturen.
     * Als er verstuurd kan worden, wordt de meegegeven string verstuurd.
     *
     * @param std::string - Het bericht dat verstuurd moet worden.
     */
    virtual void sendSocket(std::string) = 0;

    /*!
     * @brief Geeft het ontvangen bericht.
     *
     * Deze methode stuurt het ontvangen bericht, dat in de buffer van het object staat, door naar de opvrager.
     *
     * @return std::string - Het bericht dat is ontvangen.
     */
    virtual std::string received() = 0;

    /*!
     * @brief Geeft terug of er wel of niet een bericht verstuurd kan worden.
     *
     * Eerst wordt er bekeken of de socket een verbinding heeft. Is dat niet het geval, zal de verbinding opnieuw geprobeerd worden op te zetten.
     *
     * @return Boolean - Terugkoppeling op of er wel of niet een bericht verstuurd kan worden. Als er verstuurd kan worden: true.
     */
    virtual bool canSend() = 0;

    /*!
     * @brief Geeft terug of er wel of niet een bericht is ontvangen.
     *
     * Eerst wordt er bekeken of de socket een verbinding heeft. Is dat niet het geval, zal de verbinding opnieuw geprobeerd worden op te zetten.
     *
     * @return Boolean - Terugkoppeling op of er wel of niet een bericht is ontvangen. Als er is ontvangen: true.
     */
    virtual bool hasReceived() = 0;
};


#endif