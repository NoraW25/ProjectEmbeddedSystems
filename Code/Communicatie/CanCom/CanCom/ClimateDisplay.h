
/*!
 * @file ClimateDisplay.h
 * @class ClimateDisplay
 * @brief Stuurt klimaatwaarde door naar display.
 * 
 * Ontvangt de waarde van de RPiB en stuurt deze door over de 
 * CANbus.
 */

#ifndef CLIMATEDISPLAY_H
#define CLIMATEDISPLAY_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "CommunicationController.h"

class ClimateDisplay{
public:
    /*!
     * @brief Constructor ClimateDisplay
     *
     * De standard en meegegeven waarden worden meegegeven in de attributen.
     * 
     * De meegegeven controllers worden gekoppeld aan de ontvangst op:
     * De berichten met adres/ID = standaard op int parameter 1, 
     *  address_from, de RPiB voor de afstand. Gekoppelde methode:
     *  displayValue(std::vector<uint8_t>)
     * 
     * @param shared_ptr<Communication::CommunicationController> - De communicatiecontroller van de CANbus.
     * @param shared_ptr<Communication::CommunicationController> - De communicatiecontroller van de RPiB.
     * @param int - Het adres waar data van RPiB op wordt ontvangen voor het aansturen van de display.
     * @param int - Het adres waar de display naar luistert om de data weer te geven.
     */
    ClimateDisplay(std::shared_ptr<Communication::CommunicationController>, 
        std::shared_ptr<Communication::CommunicationController>,
        int, int);

    virtual ~ClimateDisplay() = default;

    /*!
     * @brief Stuurt de ontvangen klimaatwaarde door naar de display
     *
     * Ontvangt data op een adres met data, en stuurt de data door
     * naar het display op de CANbus.
     * 
     * @param vector<uint8_t> - De data die doorgestuurd wordt.
     */
    virtual void displayValue(std::vector<uint8_t>);

private:
    int address_from;
    int address_10led;

    std::shared_ptr<Communication::CommunicationController> controller_can;
    std::shared_ptr<Communication::CommunicationController> controller_rpib;

};

#endif