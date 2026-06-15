
/*!
 * @file DistanceSystemRPiB.h
 * @class DistanceSystemRPiB
 * @brief Zorgt dat de afstand wordt weergegeven.
 * 
 * De aansturing van het display, dat berekend wordt op het
 * hoofd DistanceSystem op RPiB, wordt door verstuurd op het adres
 * van de displays die de afstand weergeven.
 * 
 * Daarnaast wordt inkomende afstanddata doorgestuurd naar RPiA om
 * te verwerken.
 */

#ifndef DISTANCESYSTEMRPIB_H
#define DISTANCESYSTEMRPIB_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "CommunicationController.h"

class DistanceSystemRPiB {
public:
    /*!
     * @brief Constructor DistanceSystemRPiB
     *
     * De standard en meegegeven waarden worden meegegeven in de
     * attributen.
     * 
     * De meegegeven controllers worden gekoppeld aan de ontvangst op:
     * De berichten met adres/ID = het meegegeven adres op controller RPiB  
     * De berichten met adres/ID = 910 op de controller van de Wemos met de encoder
     * 
     * 
     * @param shared_ptr<Communication::CommunicationController> - De communicatiecontroller van de RPiA.
     * @param shared_ptr<Communication::CommunicationController> - De communicatiecontroller van de Wemos met de display.
     * @param shared_ptr<Communication::CommunicationController> - De communicatiecontroller van de Wemos met de encoder.
     * @param int - Het adres waar de data voor de display van het hoofdsysteem binnenkomt.
     */
    DistanceSystemRPiB(std::shared_ptr<Communication::CommunicationController>, 
        std::shared_ptr<Communication::CommunicationController>, 
        std::shared_ptr<Communication::CommunicationController>,
        int);

    virtual ~DistanceSystemRPiB() = default;

    /*!
     * @brief Verzend de ontvangen data voor de display door naar de display
     *
     * Wordt aangeroepen als er een bericht met adres/ID dat meegegeven wordt
     * als parameter voor het ontvangst op de display en binnenkomt op
     * de controller gekoppeld aan de Wemos met RPiB.
     * 
     * Stuurt het bericht door via de controller van de Wemos met de display. 
     * 
     * @param vector<uint9_t> - De data die door gestuurd moet worden.
     */
    virtual void sendToDisplay(std::vector<uint8_t>);

    /*!
     * @brief Verzend de ontvangen data van de encoder door naar het hoofdsysteem
     *
     * Wordt aangeroepen als er een bericht met adres/ID 910 binnenkomt op
     * de controller gekoppeld aan de Wemos met encoder.
     * 
     * Stuurt het bericht door via de controller van de RPiA.
     * 
     * @param vector<uint9_t> - De data die door gestuurd moet worden.
     */
    virtual void sendDistance(std::vector<uint8_t>);

private:
    std::shared_ptr<Communication::CommunicationController> wemos_controller;
    std::shared_ptr<Communication::CommunicationController> wemos_controller_encoder;
    std::shared_ptr<Communication::CommunicationController> rpia_controller;
    int address_display;
    int address_distance_wemos;
    int address_distance_rpia;
};

#endif