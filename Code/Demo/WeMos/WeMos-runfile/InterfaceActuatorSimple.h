
/*!
 * @file InterfaceActuatorSimple.h
 * @class InterfaceActuatorSimple
 * @brief Een interface voor WeMos om een actuator aan of uit te zetten.
 */

#ifndef INTERFACEACTUATORSIMPLE
#define INTERFACEACTUATORSIMPLE

#include <Arduino.h>

class InterfaceActuatorSimple{
public:
    /*!
     * @brief Zet de actuator aan.
     */
    virtual void turnOn() = 0;

    /*!
     * @brief Zet de actuator uit.
     */
    virtual void turnOff() = 0;

};

#endif