
/*!
 * @file Lamp.h
 * @class Lamp
 * @brief Een lamp die aan en uit kan.
 */

#ifndef LAMP_H
#define LAMP_H

#include "InterfaceActuatorSimple.h"

class Lamp : public InterfaceActuatorSimple {
public:
    /*!
     * @brief Maakt een Lamp aan.
     *
     * Maakt een lamp aan op een pinnummer van de WeMos. 
     * Standaard staat de lamp uit.
     * 
     * @param int pin_nr - het pinnummer waar de Lamp op de WeMos zit.
     */
    Lamp(int);


    void turnOn() override;
    void turnOff() override;

private:
    int pin_nr;
    bool status;
};

#endif