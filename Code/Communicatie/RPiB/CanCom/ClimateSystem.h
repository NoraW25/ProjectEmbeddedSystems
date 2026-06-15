
/*!
 * @file ClimateSystem.h
 * @class ClimateSystem
 * @brief De controlle klasse van het klimaatsysteem op de RPiB
 * 
 * Deze klasse zorgt ervoor dat het klimaatsysteem op de RPiB wordt opgezet.
 * Het maakt daarbij alle sensoren en actuatoren die het klimaat beïnvloeden aan,
 * en slaat ze op in een map. Daar staan ze gesorteerd in op type.
 * 
 * Daarnaast voert deze klasse de berekening uit, hoe de actuatoren aangestuurd
 * moeten worden op basis van de sensorwaardes.
 */

#ifndef CLIMATESYSTEM_H
#define CLIMATESYSTEM_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "ClimateSensor.h"
#include "ClimateActuatorPWM.h"
#include "CommunicationController.h"


class ClimateSystem{
public:
    /*!
     * @brief Constructor ClimateSystem
     *
     * De bekende sensoren en actuatoren worden hier toegevoegd aan het systeem.
     * 
     * 
     * @param shared_ptr<Communication::CommunicationController> - De communicatiecontroller van de Wemos.
     * @param shared_ptr<Communication::CommunicationController> - De communicatiecontroller van de RPiA.
     * @param int - Het adres waarop het alarm te bereiken is.
     */
    ClimateSystem(std::shared_ptr<Communication::CommunicationController>, std::shared_ptr<Communication::CommunicationController>, int);

    virtual ~ClimateSystem() = default;

    /*!
     * @brief Berekent de standen van de actuatoren.
     *
     * Van alle sensoren wordt de recentste waarde opgevraagd.
     * De waardes van sensoren van hetzelfde type wordt het gemiddelde van genomen.
     * Per gemiddelde wordt een waarde van een actuator toegekend.
     * Vervolgens wordt de load voor de actuatoren berekend.
     * Als er meer van hetzelfde soort type ventilatoren zijn, 
     * wordt de load gedeelt door het aantal ventilatoren.
     *
     * Als de waarde van CO2 te hoog wordt wordt er een waarschuwing afgegeven.
     * De gemiddelde luchtvochtigheid wordt verstuurd naar RPiA om daar gedisplayt te worden.
     */
    virtual void calculateSettings();

    /*!
     * @brief Voegt een sensor toe aan het klimaatsysteem.
     *
     * Voegt een klimaatsensor toe aan het klimaatsysteem.
     * De sensor wordt netjes in de goede catagorie geplaatst.
     * 
     * @param string - std::string die het type sensor aangeeft.
     * @param shared_ptr<ClimateSensor> - Een std::shared_ptr die de toe te voegen klimaatsensor meegeeft.
     */
    virtual void addSensor(std::string, std::shared_ptr<ClimateSensor>);

    /*!
     * @brief Voegt een actuator toe aan het klimaatsysteem.
     *
     * Voegt een klimaatactuator toe aan het klimaatsysteem.
     * De actuator wordt netjes in de goede catagorie geplaatst.
     * 
     * @param string - std::string die het type sensor aangeeft.
     * @param shared_ptr<ClimateActuatorPWM> - Een std::shared_ptr die de toe te voegen klimaatactuator meegeeft.
     */
    virtual void addActuatorPWM(std::string, std::shared_ptr<ClimateActuatorPWM>);

private:
    std::shared_ptr<Communication::CommunicationController> controller;
    std::shared_ptr<Communication::CommunicationController> controller_rpia;
    std::map<std::string, std::vector<std::shared_ptr<ClimateSensor>>> sensors;
    std::map<std::string, std::vector<std::shared_ptr<ClimateActuatorPWM>>> pwm_actuators;

    std::map<uint16_t, std::shared_ptr<ClimateSensor>> sensor_address_map;

    int buzzer_address;
};

#endif