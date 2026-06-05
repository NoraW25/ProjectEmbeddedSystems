
#include "LampPWM.h"

LampPWM::LampPWM(int pinNr):
    pin_nr(pinNr),
    status(false),
    dutycycle(0){

    pinMode(pin_nr, OUTPUT);
    digitalWrite(pin_nr, LOW);
}

void LampPWM::turnOn(){
    status = true;
    createPWM();
}

void LampPWM::turnOn(int new_dutycycle){
    dutycycle = map(new_dutycycle, 0, 100, 0, 255);
    status = true;
    createPWM();
}

void LampPWM::turnOff(){
    status = false;
    digitalWrite(pin_nr, LOW);
}

void LampPWM::changeDutycycle(int new_dutycycle){
    dutycycle = map(new_dutycycle, 0, 100, 0, 255);

    if (status){
        createPWM();
    }
}

void LampPWM::createPWM(){
    analogWrite(pin_nr, dutycycle);
}