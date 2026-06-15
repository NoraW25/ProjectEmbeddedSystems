
/*!
 * @file DistanceSystem.h
 * @class DistanceSystem
 * @brief Zorgt dat de afstand wordt verwerkt en weergegeven.
 * 
 * De aansturing van het display van de afstand, het ontvangt
 * om de zoveel afstand een bericht van de afstandmeter.
 * Deze telt die bij elkaar op en rekent uit wat er op de display
 * moet komen en stuurt dit vervolgens door naar het display.
 * 
 * Als er op de resetknop wordt gedrukt wordt de afstand op 0
 * gezet. En wordt de display ook daartoe aangepast.
 */

#ifndef DISTANCESYSTEM_H
#define DISTANCESYSTEM_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "CommunicationController.h"

class DistanceSystem{
public:
    /*!
     * @brief Constructor DistanceSystem
     *
     * De standard en meegegeven waarden worden meegegeven in de attributen.
     * 
     * De meegegeven controllers worden gekoppeld aan de ontvangst op:
     * De berichten met adres/ID = standaard op 512, 
     *  de cancontroller voor de afstand.
     * De berichten met adres/ID = standaard op 511, 
     *  de cancontroller voor de resetknop.
     * De berichten met adres/ID = standaard op 512, 
     *  de RPiBcontroller voor de afstand.
     * 
     * @param shared_ptr<Communication::CommunicationController> - De communicatiecontroller van de CANbus.
     * @param shared_ptr<Communication::CommunicationController> - De communicatiecontroller van de RPiB.
     */
    DistanceSystem(std::shared_ptr<Communication::CommunicationController>, 
        std::shared_ptr<Communication::CommunicationController>);

    /*!
     * @brief Constructor DistanceSystem
     *
     * De standard en meegegeven waarden worden meegegeven in de attributen.
     * 
     * De meegegeven controllers worden gekoppeld aan de ontvangst op:
     * De berichten met adres/ID = standaard op int parameter 1, turns, 
     *  de cancontroller voor de afstand.
     * De berichten met adres/ID = standaard op int parameter 2, reset_button, 
     *  de cancontroller voor de resetknop.
     * De berichten met adres/ID = standaard op int parameter 1, turns, 
     *  de RPiBcontroller voor de afstand.
     * 
     * @param shared_ptr<Communication::CommunicationController> - De communicatiecontroller van de CANbus.
     * @param shared_ptr<Communication::CommunicationController> - De communicatiecontroller van de RPiB.
     */    
    DistanceSystem(std::shared_ptr<Communication::CommunicationController>, 
        std::shared_ptr<Communication::CommunicationController>, int, int, int);

    virtual ~DistanceSystem() = default;

    /*!
     * @brief Voegt afstand toe aan de afstandteller.
     *
     * Krijgt via de controller een bericht dat de afstand is vergroot.
     * Telt het aantal keer dat de controller zo'n bericht geeft.
     * 
     * Laat het aantal lampjes dat aan moet berekenen en verzonden met de 
     * calculateAmountOfLights() methode.
     * 
     * @param vector<uint8_t> - De meegegeven data, wordt nu niks meegedaan.
     */ 
    virtual void addToDistance(std::vector<uint8_t>);

    /*!
     * @brief Reset de afstand.
     *
     * De afstand wordt gelijk gezet aan 0;
     * De hoeveelheid lampjes die aanmoeten wordt weer berekent en 
     * verzonden via de methode calculateAmountOfLights().
     * 
     * @param vector<uint8_t> - De meegegeven data, wordt nu niks meegedaan.
     */ 
    virtual void resetDistance(std::vector<uint8_t>);

    /*!
     * @brief Berekent hoeveel lampjes van het display aan moeten.
     *
     * Er wordt een berekening gedaan hoeveel lampjes van de progressiebar
     * aangezet moeten worden, dit wordt in een bericht verstuurd naar de
     * RPiB om het daar op een display te laten vertonen.
     */
    virtual void calculateAmountOfLights();


private:
    int distance;
    int amount_of_lights;
    int finish_distance;
    int amount_of_lights_available;

    int address_display;
    int address_reset_button;
    int address_turns;

    std::shared_ptr<Communication::CommunicationController> can_controller;
    std::shared_ptr<Communication::CommunicationController> rpib_controller;

};

#endif