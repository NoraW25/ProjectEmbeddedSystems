
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


class Socket{
public:
    /*!
     * @brief Verstuurd een bericht.
     *
     * Deze methode bekijkt of er verstuurd kan worden met kanVersturen.
     * Als er verstuurd kan worden, wordt de meegegeven string verstuurd.
     *
     * @param int adres of proces id, geeft aan wat voor soort data over de lijn gestuurd wordt en wie erop moet reageren.
     * @param vector<uint8_t> de data die verzonden moet worden.
     */
    virtual void send(int, std::vector<uint8_t>) = 0;


    /*!
     * @brief Geeft terug of er wel of niet een bericht verstuurd kan worden.
     *
     * Eerst wordt er bekeken of de socket een verbinding heeft. Is dat niet het geval, zal de verbinding opnieuw geprobeerd worden op te zetten.
     *
     * @return Boolean - Terugkoppeling op of er wel of niet een bericht verstuurd kan worden. Als er verstuurd kan worden: true.
     */
    virtual bool canSend() = 0;

    /*!
     * @brief Geeft terug of er wel of niet een bericht is ontvangen. Als er een bericht is ontvangen wordt de inhoud meegegeven.
     *
     * Eerst wordt er bekeken of de socket een verbinding heeft. Is dat niet het geval, zal de verbinding opnieuw geprobeerd worden op te zetten.
     * Als er een bericht is ontvangen worden het adres en de data meegegeven in met de pointerparameters.
     *
     * @param int   adres of proces id, geeft aan wat voor soort data over de lijn gestuurd wordt en wie erop moet reageren.
     * @param vector<uint8_t> de opslagplaats voor de ontvangen data.
     * @return Boolean - Terugkoppeling op of er wel of niet een bericht is ontvangen. Als er is ontvangen: true.
     */
    virtual bool received(int*, std::vector<uint8_t>*) = 0;
};


#endif