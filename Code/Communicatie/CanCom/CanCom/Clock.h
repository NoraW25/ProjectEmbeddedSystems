
/*!
 * @file Clock.h
 * @class Clock
 * @brief Stuurt de klok op de CANbus periodiek updates.
 * 
 * Updates de klok op de CANbus periodiek met de accurate lokale tijd.
 */

#ifndef CLOCK_H
#define CLOCK_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "CommunicationController.h"

class Clock {
public:
    /*!
     * @brief Constructor Clock
     *
     * De standard en meegegeven waarden worden meegegeven in de attributen.
     * 
     * Via de cancontroller wordt de runservice opgehaald en er wordt een 
     * bericht ingepland over een seconden om de tijd te updaten via 
     * methode updateClock().
     * 
     * @param int - Het adres waar de klok op de CAN naar luistert.
     * @param shared_ptr<Communication::CommunicationController> - De communicatiecontroller van de CANbus.
     */
    Clock(int, std::shared_ptr<Communication::CommunicationController>);

    virtual ~Clock() = default;

    /*!
     * @brief Stuurt periodiek een tijd update
     *
     * Vraagt de locale tijd op en stuurt deze op de CANbus met het
     * aangegeven adres.
     * 
     * Plant om nog een keer te zenden over een seconden, door
     * nog een keer deze methode mee te geven in de runservice.
     */
    void updateClock();

private:
    int address_clock;
    std::shared_ptr<Communication::CommunicationController> controller;
    Scheduling::RunServiceController* runservice;
};

#endif