
/*!
 * @file InterfaceActuatorPWM.h
 * @class InterfaceActuatorPWM
 * @brief Een interface voor WeMos om een PWM signaal te creëren op een actuator.
 */

#ifndef INTERFACEACTUATORPWM
#define INTERFACEACTUATORPWM

#include <Arduino.h>

class InterfaceActuatorPWM{
public:
    /*!
     * @brief Zet de actuator aan.
     *
     * Als deze functie aangeroepen wordt, gaat de actuator aan op de dutycycle die onthouden staat in het object.
     */
    virtual void turnOn() = 0;

    /*!
     * @brief Zet de actuator aan.
     *
     * Zet de actuator aan op de dutycycle die wordt meegegeven.
     * 
     * @param int - Dutycycle, kan een waarde hebben van 0 t/m 100.
     */
    virtual void turnOn(int) = 0;

    /*!
     * @brief Zet de actuator uit.
     * 
     * Zet de actuator uit zonder de dutycycle te veranderen.
     */
    virtual void turnOff() = 0;

    /*!
     * @brief Veranderd de dutycycle.
     *
     * Veranderd de dutycycle, deze wordt in het object opgeslagen.
     * Als de actuator aan staat wordt de dutycycle meteen ingeladen, 
     * anders blijft de actuator uit en wordt het alleen opgeslagen in het object voor als de actuator weer ingeschakeld wordt.
     * 
     * @param int - Dutycycle, kan een waarde hebben van 0 t/m 100.
     */
    virtual void changeDutycycle(int) = 0;
};

#endif