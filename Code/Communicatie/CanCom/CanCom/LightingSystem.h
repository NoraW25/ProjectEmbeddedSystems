
/*!
 * @file LightingSystem.h
 * @class LightingSystem
 * @brief Regelt de verlichting.
 * 
 * Zorgt ervoor dat de verlichting aan/uit gezet kan worden.
 * Daarnaast kan de kleur van het licht worden aangepast.
 */

#ifndef LIGHTING_H
#define LIGHTING_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "CommunicationController.h"

class LightingSystem {
public:
    /*!
     * @brief Constructor LightingSystem
     *
     * De standard en meegegeven waarden worden meegegeven in de attributen.
     * 
     * De meegegeven controllers worden gekoppeld aan de ontvangst op:
     * De berichten met adres/ID = standaard op int parameter 1, changeColour(), 
     *  de cancontroller voor de verander kleur knop.
     * De berichten met adres/ID = standaard op int parameter 2, toggleLight(), 
     *  de cancontroller voor de aan/uit knop.
     * 
     * Daarnaast worden alle kleuren in een map geplaatst en de iterator op
     * de eeste plek in de map gezet.
     * 
     * @param int - Het adres waar veranderingen voor de kleur van het licht op binnenkomen.
     * @param int - Het adres waar veranderingen van de status van het licht op binnenkomen.
     * @param shared_ptr<Communication::CommunicationController> - De communicatiecontroller van de CANbus.
     */
    LightingSystem(int, int, std::shared_ptr<Communication::CommunicationController>);

    virtual ~LightingSystem() = default;


    /*!
     * @brief Verandert de kleur van het licht.
     *
     * Checkt eerst of het licht aanstaat, zo ja volgt de rest:
     *
     * Schuift de iterator van de map 1 plek op en checkt of die opnieuw
     * door de map moet loopen om naar de volgende kleur te gaan.
     * 
     * Roept daarna sendNewColour() aan om de nieuwe kleur ook echt te 
     * versturen.
     * 
     * @param vector<uint8_t> - De meegegeven data, wordt nu niks meegedaan alleen meegegeven.
     */ 
    virtual void changeColour(std::vector<uint8_t>);

    /*!
     * @brief Zet het licht aan/uit.
     *
     * Zet de lichten uit wanner ze aan zijn, en zet de lichten aan wanneer ze uit zijn.
     * Wanneer er de lichten aan gezet worden, wordt de methode sendNewColour() aangeroepen.
     * Anders verzend deze methode zelf een bericht op de CANbus op het adres waar het licht
     * naar luistert als het uit gezet moet worden.
     * 
     * @param vector<uint8_t> - De meegegeven data, wordt nu niks meegedaan alleen meegegeven.
     */
    virtual void toggleLight(std::vector<uint8_t>);

    /*!
     * @brief Verzend een bericht om een kleur aan te zetten.
     *
     * Elke kleur luistert naar zijn eigen adres, deze staan vast in de map
     * De iterator geeft aan welke kleur aan de beurt is, als value heeft
     * de kleur een adres.
     * 
     * Op dat adres wordt in deze methode een bericht verstuurd op de CANbus via de cancontroller.
     */
    virtual void sendNewColour();

private:
    std::map<std::string, int>::iterator current_colour;
    std::shared_ptr<Communication::CommunicationController> controller;
    std::map<std::string, int> colours;

    int button_address;
    int lights_off_address;
    bool lights_on;
};

#endif