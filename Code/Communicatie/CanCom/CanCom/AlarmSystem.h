
/*!
 * @file AlarmSystem.h
 * @class AlarmSystem
 * @brief Laat een alarm afgaan.
 * 
 * Zorgt ervoor dat er een alarm afgaat als er een bericht
 * van de RPiB met een specifiek adres binnenkomt.
 */


#ifndef ALARMSYSTEEM_H
#define ALARMSYSTEEM_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "CommunicationController.h"

class AlarmSystem {
public:
    /*!
     * @brief Constructor DistanceSystem
     *
     * De standard en meegegeven waarden worden meegegeven in de attributen.
     * 
     * De meegegeven controllers worden gekoppeld aan de ontvangst op:
     * De berichten met adres/ID = adres meegegeven in de int parameter,
     *  koppelt de methode soundAlarm(std::vector<uint8_t>) eraan vast.
     * 
     * @param shared_ptr<Communication::CommunicationController> - De communicatiecontroller van de CANbus.
     * @param shared_ptr<Communication::CommunicationController> - De communicatiecontroller van de RPiB.
     * @param int - Het adres/ID waar het alarm op doorgegeven wordt.
     */
    AlarmSystem(std::shared_ptr<Communication::CommunicationController>, std::shared_ptr<Communication::CommunicationController>, int);

    virtual ~AlarmSystem() = default;

    /*!
     * @brief Stuurt het alarmsignaal door.
     *
     * Wordt aangeroepen als er vanaf de RPiB een alarmsignaal wordt gegeven
     * op het meegegeven adres. Verzend op het zelfde adres op de CANbus een
     * bericht die de buzzer laat afgaan.
     * 
     * @param vector<uint8_t> - De meegegeven data, wordt nu niks meegedaan alleen meegegeven.
     */ 
    virtual void soundAlarm(std::vector<uint8_t>);

private:
    int alarm_address;
    std::shared_ptr<Communication::CommunicationController> controller_can;
    std::shared_ptr<Communication::CommunicationController> controller_rpib;

};

#endif