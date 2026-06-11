
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
                            // Om this af te vangen en alleen het type void (*)(std::vector<uint8_t>) mee te geven ipv void (ClimateSensor::*)(std::vector<uint8_t>)
    );
}

void AlarmSystem::soundAlarm(std::vector<uint8_t> data){
    std::cout<<"In alarm systeem"<<std::endl;
    controller_can->transmitData(alarm_address, data);
}