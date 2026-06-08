
#include "ClimateActuatorPWM.h"



ClimateActuatorPWM::ClimateActuatorPWM(int address, std::shared_ptr<Communication::CommunicationController> controller):
    status(0),
    address(address),
    controller(controller){

}

void ClimateActuatorPWM::setPWM(int dutycycle){
    std::vector<uint8_t> data;
    int amount_of_bytes = 4;
    
    // LSB eerst
    for (int i = 0; i < amount_of_bytes; i++){
        uint8_t byte_value = (dutycycle >> (8*i)) & 0xFF;
        data.push_back(byte_value);
    }
    
    controller->transmitData(address, data);
}