
#ifndef CLIMATEACTUATORPWM_H
#define CLIMATEACTUATORPWM_H


#include <memory>
#include "Code\Communicatie\RPiB\CanCom\CommunicationController.h"

class ClimateActuatorPWM {
public:
    ClimateActuatorPWM(int address, std::shared_ptr<Communication::CommunicationController>);

    virtual ~ClimateActuatorPWM() = default;

    virtual void setPWM(int);


private:
    int status;
    unsigned int address;
    std::shared_ptr<Communication::CommunicationController> controller;
};

#endif