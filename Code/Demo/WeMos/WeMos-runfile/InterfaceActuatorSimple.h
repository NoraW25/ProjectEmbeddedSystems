

#ifndef INTERFACEACTUATORSIMPLE
#define INTERFACEACTUATORSIMPLE

#include <Arduino.h>

class InterfaceActuatorSimple{
public:
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;

};

#endif