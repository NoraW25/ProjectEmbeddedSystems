
#include "AlarmSystem.h"

#include <iostream>

AlarmSystem::AlarmSystem(std::shared_ptr<Communication::CommunicationController> controller_can, 
    std::shared_ptr<Communication::CommunicationController> controller_rpib, 
    int address):
        alarm_address(address),
        controller_can(controller_can),
        controller_rpib(controller_rpib){

            controller_rpib->logReceived(alarm_address,
                            [this](std::vector<uint8_t> data)
                            { soundAlarm(data); }
    );
}

void AlarmSystem::soundAlarm(std::vector<uint8_t> data){
    controller_can->transmitData(alarm_address, data);
}