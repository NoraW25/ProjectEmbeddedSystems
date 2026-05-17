
#ifndef LAMPPWM_H
#define LAMPPWM_H

#include "InterfaceActuatorPWM.h"

class LampPWM : public InterfaceActuatorPWM {
public:
    LampPWM(int);
    void turnOn() override;
    void turnOn(int) override;
    void turnOff() override;
    void changeDutycycle(int) override;

private:
    int pin_nr;
    bool status;
    int dutycycle;

    void createPWM();
};

#endif