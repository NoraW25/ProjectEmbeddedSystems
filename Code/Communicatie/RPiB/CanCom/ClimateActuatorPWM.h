
/*!
 * @file ClimateActuatorPWM.h
 * @class ClimateActuatorPWM
 * @brief De actuator klasse van het klimaatsysteem op de RPiB
 * 
 * Deze klasse zorgt ervoor dat de berekende actuatorstanden in de
 * klimaatsysteem klasse worden doorgevoerd naar de klimaatactuatoren.
 * 
 * In de klasse wordt er een bericht verstuurd naar het adres waar de
 * actuator op gekoppeld zit. Dit wordt met de controller gedaan die
 * bij het aanmaken van de klimaatactuator meegegeven is.
 */

#ifndef CLIMATEACTUATORPWM_H
#define CLIMATEACTUATORPWM_H

#include <memory>
#include "CommunicationController.h"


class ClimateActuatorPWM {
public:
    /*!
     * @brief Constructor ClimateActuatorPWM
     *
     * Het standaard aanmaken van de klasse. 
     * Attributen krijgen de meegegeven of standaard waarden.
     * Voor de rest gebeurd er niks.
     * 
     * @param int - Het adres waar de actuator op te benaderen is.
     * @param shared_ptr<Communication::CommunicationController> - De communicatiecontroller waar de actuator achter zit.
     */
    ClimateActuatorPWM(int address, std::shared_ptr<Communication::CommunicationController>);

    virtual ~ClimateActuatorPWM() = default;

    /*!
     * @brief Verstuurd het PWM signaal naar de
     *
     * Deze methode verstuurd een array met unsigned integers van 8 bits.
     * De grootte van de vector is gelijk aan het aantal bytes in een int.
     * Per 8 bits uit de integer gehaald, en verschoven zodat het bericht LSB
     * first in de vector komt te staan.
     * In de Wemos wordt de vector ontleed met LSB first, zodat de juiste dutycycle
     * er weer uitkomt.
     * 
     * @param int - De dutycycle waarde waar de actuator op moet draaien.
     */
    virtual void setPWM(int);


private:
    int status;
    unsigned int address;
    std::shared_ptr<Communication::CommunicationController> controller;
};

#endif