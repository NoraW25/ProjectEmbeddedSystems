
#include "Lamp.h"

Lamp::Lamp(int pinNr):
    pin_nr(pinNr),
    status(false){
    pinMode(pin_nr, OUTPUT);
    digitalWrite(pin_nr, LOW);
}

void Lamp::turnOn(){
    if (!status){
        digitalWrite(pin_nr, HIGH);
        status = true;
    }    
}

void Lamp::turnOff(){
    if (status){
        digitalWrite(pin_nr, LOW);
        status = false;
    }    
}
