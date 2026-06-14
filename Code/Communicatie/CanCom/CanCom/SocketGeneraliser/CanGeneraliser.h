
/*!
 * @file CanGeneraliser.h
 * @class CanGeneraliser
 * @brief Maakt een CANbus socket aan of haalt de instantie ervan op en vertaalt
 * de functies van de socket naar functies die gebruikt worden in het communicatie-
 * systeem op de Raspberry Pi.
 * 
 * De abstracte methodes uit de klassen Communication::Receiver en 
 * Communication::Transmitter worden hiervoor overschreven. In de methodes worden
 * CanSocket methodes aangeroepen.
 */


#ifndef CANGENERALISER_H
#define CANGENERALISER_H

#include "../Receiver.h"
#include "../Transmitter.h"
#include "../Socket/CanSocket.h"


class CanGeneraliser : 
    public Communication::Receiver, public Communication::Transmitter
{
public:
    /*!
    * @brief Maakt de CanSocket aan of haalt de instantie ervan op.
    */
    CanGeneraliser();

    /*!
    * @brief Maakt de CanSocket aan of haalt de instantie ervan op.
    * 
    * @param string - std::string de CANbus interface waar een socket op wordt geprobeert te maken
    */
    CanGeneraliser(std::string);

    virtual ~CanGeneraliser() = default;

    virtual void sendData(int, std::vector<uint8_t>) override;
    virtual void sendRequest(int) override;
    virtual bool receive(int*, std::vector<uint8_t>*) override;

private:
    CanSocket* socket;    
};


#endif