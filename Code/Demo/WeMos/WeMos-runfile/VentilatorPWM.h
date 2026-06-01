
/*!
 * @file VentilatorPWM.h
 * @class VentilatorPWM
 * @brief Een ventilator die werkt op een PWM signaal en dus kan dimmen.
 */

#ifndef VENTILATORPWM_H
#define VENTILATORPWM_H

#include "InterfaceActuatorPWM.h"

class VentilatorPWM : public InterfaceActuatorPWM {
public:
    /*!
     * @brief Maakt een VentilatorPWM aan.
     *
     * Maakt een ventilator aan op een pinnummer van de WeMos. 
     * Standaard staat de ventilator uit en de dutycycle op 0.
     * 
     * @param int pin_nr - het pinnummer waar de ventilator op de WeMos zit.
     */
    VentilatorPWM(int);


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