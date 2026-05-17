
#ifndef LAMP_H
#define LAMP_H

#include "InterfaceActuatorSimple.h"

class Lamp : public InterfaceActuatorSimple {
public:
    Lamp(int);
    void turnOn() override;
    void turnOff() override;

private:
    int pin_nr;
    bool status;
};

#endif