

#ifndef INTERFACEACTUATORPWM
#define INTERFACEACTUATORPWM

#include <Arduino.h>

class InterfaceActuatorPWM{
public:
    virtual void turnOn() = 0;
    virtual void turnOn(int) = 0;
    virtual void turnOff() = 0;
    virtual void changeDutycycle(int) = 0;
};

#endif