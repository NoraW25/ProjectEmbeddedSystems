
/*!
 * @file ClimateSensor.h
 * @class ClimateSensor
 * @brief Klimaatsensor voor in het klimaatsysteem op de RPiB
 * 
 * Deze klasse zorgt ervoor dat de klimaatwaarden uit een klimaatsensor 
 * afkomstig van een Wemos microcontroller op de RPiB goed verwerkt worden.
 * 
 * Als de Wemos een bericht op het meegegeven adres stuurt, zorgt de RPiB 
 * controller ervoor dat de meegegeven data in deze via een methode verwerkt
 * kunnen worden. De data wordt hier in die methode uit de vector gehaald en
 * opgeslagen.
 */

#ifndef CLIMATESENSOR_H
#define CLIMATESENSOR_H

#include <memory>
#include <vector>
#include "CommunicationController.h"

class ClimateSystem;

class ClimateSensor {
public:
    /*!
     * @brief Constructor ClimateSensor
     *
     * De setCurrentValue() methode wordt gekoppeld aan de controller, op het meegegeven adres.
     * 
     * @param int - Het adres waar data op binnenkomt.
     * @param shared_ptr<Communication::CommunicationController> - De communicatiecontroller van de Wemos.
     * @param ClimateSystem* - Het klimaatsysteem, deze is nodig om de berekening uit te laten voeren.
     */
    ClimateSensor(int, std::shared_ptr<Communication::CommunicationController>, ClimateSystem*);
    
    virtual ~ClimateSensor() = default;

    /*!
     * @brief Zet de value van de klimaatsensor op het meegegeven aantal.
     *
     * Deze methode verwacht een array met unsigned integers van 8 bits.
     * De grootte van de vector is gelijk aan het aantal bytes in een double.
     * De data wordt eruit gehaald en achter elkaar gezet, zodat de double die het
     * was in de Wemos weer terug verschijnt.
     * In de Wemos wordt de double ontleed met LSB first, dus die staat op plaats 0 van
     * de vector. En wordt in deze methode ook weer zo opgebouwd worden.
     * 
     * Vervolgens roept de methode een andere methode in ClimateSystem aan om de nieuwe
     * instellingen voor de aparatuur te bepalen die invloed hebben op het klimaat, bv
     * de ventilator. 
     * 
     * De status wordt hier gereset.
     * 
     * @param vector<uint8_t> - Een std::vector gevuld met data, een double is in 8 dataslots LSB eerst ontleed en in de vector gezet.
     */
    virtual void setCurrentValue(std::vector<uint8_t>);

    /*!
     * @brief Haalt de laatste waarde van de sensor op.
     *
     * Geeft de laatst binnengekregen waarde van de klimaatsensor die via de Wemos is
     * binnengekomen.
     * 
     * Bij de status wordt hier 1 opgetelt.
     * 
     * @return double - Laatste binnengekregen waarde van de klimaatsensor.
     */
    virtual double getCurrentValue();

    /*!
     * @brief Haalt de status van het systeem op.
     * 
     * Elke keer dat de value van de sensor door iets wordt uitgelezen wordt de waarde
     * van de status verhoogd. Zo wordt er bijgehouden hoevaak de sensor is aangeroepen
     * ten opzichte van dat die is geupdate. Hierdoor kan er nagegaan worden of de waarde
     * nog enigzins relevant is.
     * 
     * @return int - Geeft het aantal keer dat de waarde is opgehaald na de laatste update.
     */
    virtual int getStatus();

private:
    uint8_t address;
    double value;
    int status;
    std::shared_ptr<Communication::CommunicationController> controller;
    std::shared_ptr<Communication::CommunicationController> controller_rpia;
    ClimateSystem* system;		
};


#endif