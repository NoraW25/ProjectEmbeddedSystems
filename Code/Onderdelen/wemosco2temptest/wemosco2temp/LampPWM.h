
/*!
 * @file LampPWM.h
 * @class LampPWM
 * @brief Een lamp die werkt op een PWM signaal en dus kan dimmen.
 */

#ifndef LAMPPWM_H
#define LAMPPWM_H

#include "InterfaceActuatorPWM.h"

class LampPWM : public InterfaceActuatorPWM {
public:
    /*!
     * @brief Maakt een LampPWM aan.
     *
     * Maakt een lamp aan op een pinnummer van de WeMos. 
     * Standaard staat de lamp uit en de dutycycle op 0.
     * 
     * @param int pin_nr - het pinnummer waar de Lamp op de WeMos zit.
     */
    LampPWM(int);


    void turnOn() override;
    void turnOn(int) override;
    void turnOff() override;
    void changeDutycycle(int) override;

private:
    int pin_nr;
    bool status;
    int dutycycle;

    /*!
     * @brief Maakt een PWM signaal op pin_nr.
     *
     * Maakt een PWM signaal op pin_nr met de dutycylce die het object heeft.
     */
    void createPWM();
};

#endif