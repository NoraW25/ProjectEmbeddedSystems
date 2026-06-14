
/*!
 * @file ClientGeneraliser.h
 * @class ClientGeneraliser
 * @brief Maakt een ClientSocket aan en vertaalt de functies van de socket 
 * naar functies die gebruikt worden in het communicatiesysteem op de Raspberry Pi.
 * 
 * De abstracte methodes uit de klassen Communication::Receiver en 
 * Communication::Transmitter worden hiervoor overschreven. In de methodes worden
 * ClientSocket methodes aangeroepen.
 */

#ifndef CLIENTGENERALISER_H
#define CLIENTGENERALISER_H

#include "../Receiver.h"
#include "../Transmitter.h"
#include "../Socket/ClientSocket.h"


class ClientGeneraliser : 
    public Communication::Receiver, public Communication::Transmitter
{
public:
    /*!
    * @brief Maakt de ClientSocket aan.
    *
    * @param string - std::string hier wordt het ipadres van de server waar contact mee wordt gezocht in megegeven.
    * @param bool - geeft aan of de client een Wemos is of niet, True voor wel een Wemos, False voor geen Wemos. Standaard wordt er geen Wemos verwacht.
    */
    ClientGeneraliser(std::string, bool);
    
    virtual ~ClientGeneraliser() = default;

    virtual void sendData(int, std::vector<uint8_t>) override;
    virtual void sendRequest(int) override;
    virtual bool receive(int*, std::vector<uint8_t>*) override;

private:
    ClientSocket socket;    
};


#endif