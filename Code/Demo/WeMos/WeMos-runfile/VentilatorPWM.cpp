
#include "VentilatorPWM.h"

VentilatorPWM::VentilatorPWM(int pinNr):
    pin_nr(pinNr),
    status(false),
    dutycycle(0){

    pinMode(pin_nr, OUTPUT);
    digitalWrite(pin_nr, LOW);
}

void VentilatorPWM::turnOn(){
    status = true;
    createPWM();
}

void VentilatorPWM::turnOn(int new_dutycycle){
    dutycycle = map(new_dutycycle, 0, 100, 0, 150);
    status = true;
    createPWM();
}

void VentilatorPWM::turnOff(){
    status = false;
    digitalWrite(pin_nr, LOW);
}

void VentilatorPWM::changeDutycycle(int new_dutycycle){
    dutycycle = map(new_dutycycle, 0, 100, 0, 150);

    if (status){
        createPWM();
    }
}

void VentilatorPWM::createPWM(){
    analogWrite(pin_nr, dutycycle);
}