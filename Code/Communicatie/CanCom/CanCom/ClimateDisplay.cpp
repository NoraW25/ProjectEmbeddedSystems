
#include "ClimateDisplay.h"


ClimateDisplay::ClimateDisplay(std::shared_ptr<Communication::CommunicationController> controller_can, 
    std::shared_ptr<Communication::CommunicationController> controller_rpib,
    int address_from, int address_10led):
        controller_can(controller_can),
        controller_rpib(controller_rpib),
        address_from(address_from),
        address_10led(address_10led)
{
    controller_rpib->logReceived(address_from,
                            [this](std::vector<uint8_t> data)
                            { displayValue(data); });
}

void ClimateDisplay::displayValue(std::vector<uint8_t> data){
    controller_can->transmitData(address_10led, data);
}