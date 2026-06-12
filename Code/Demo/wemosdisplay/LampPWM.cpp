
/**
 * @file LampPWM.cpp
 * @brief Implementatie van `LampPWM` - PWM-besturing voor lampen op Wemos.
 */

#include "LampPWM.h"

/**
 * @brief Constructor: initialiseert het pin-nummer en zet lamp uit.
 * @param pinNr Pinnummer van de Lamp op de Wemos
 */
LampPWM::LampPWM(int pinNr) : pin_nr(pinNr),
                              status(false),
                              dutycycle(0)
{

    pinMode(pin_nr, OUTPUT);
    digitalWrite(pin_nr, LOW);
}

/**
 * @brief Zet de lamp aan met de huidige dutycycle.
 */
void LampPWM::turnOn()
{
    status = true;
    createPWM();
}

/**
 * @brief Zet de lamp aan en stel een nieuwe dutycycle in (0-100%).
 * @param new_dutycycle Percentage dimniveau (0-100)
 */
void LampPWM::turnOn(int new_dutycycle)
{
    dutycycle = map(new_dutycycle, 0, 100, 0, 255);
    status = true;
    createPWM();
}

/**
 * @brief Zet de lamp uit en stopt PWM.
 */
void LampPWM::turnOff()
{
    status = false;
    digitalWrite(pin_nr, LOW);
}

/**
 * @brief Wijzigt de dutycycle; past directe PWM aan als lamp aan is.
 * @param new_dutycycle Percentage dimniveau (0-100)
 */
void LampPWM::changeDutycycle(int new_dutycycle)
{
    dutycycle = map(new_dutycycle, 0, 100, 0, 255);

    if (status)
    {
        createPWM();
    }
}

/**
 * @brief Schrijft de actuele dutycycle naar de pin (0-255).
 */
void LampPWM::createPWM()
{
    analogWrite(pin_nr, dutycycle);
}