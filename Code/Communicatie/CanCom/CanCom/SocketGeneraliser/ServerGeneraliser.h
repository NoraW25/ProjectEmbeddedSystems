
/*!
 * @file ServerGeneraliser.h
 * @class ServerGeneraliser
 * @brief Maakt een ServerSocket aan of haalt de instantie ervan op en vertaalt de
 * methodes van de socket naar methodes die gebruikt worden in het communicatie-
 * systeem op de Raspberry Pi.
 * 
 * De abstracte methodes uit de klassen Communication::Receiver en 
 * Communication::Transmitter worden hiervoor overschreven. In de methodes worden
 * ServerSocket methodes aangeroepen.
 */

#ifndef SERVERGENERALISER_H
#define SERVERGENERALISER_H

#include "../Receiver.h"
#include "../Transmitter.h"
#include "../Socket/ServerSocket.h"


class ServerGeneraliser : 
    public Communication::Receiver, public Communication::Transmitter
{
public:
    /*!
    * @brief Maakt de ServerSocket aan of haalt de instantie ervan op.
    * 
    * Het portnummer waar de server voor clients op te bereiken zal zijn, 
    * staat standaard op 8080.
    */
    ServerGeneraliser();

    /*!
    * @brief Maakt de ServerSocket aan of haalt de instantie ervan op.
    * 
    * @param int - het portnummer waar de server op te bereiken zal zijn.
    */
    ServerGeneraliser(int);
    
    virtual ~ServerGeneraliser() = default;

    virtual void sendData(int, std::vector<uint8_t>) override;
    virtual void sendRequest(int) override;
    virtual bool receive(int*, std::vector<uint8_t>*) override;

private:
    ServerSocket* socket;    
};


#endif