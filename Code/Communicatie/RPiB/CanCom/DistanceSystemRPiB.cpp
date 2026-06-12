
#include "DistanceSystemRPiB.h"

DistanceSystemRPiB::DistanceSystemRPiB(std::shared_ptr<Communication::CommunicationController> wemos_controller, 
    std::shared_ptr<Communication::CommunicationController> rpia_controller, 
    int address):    
        wemos_controller(wemos_controller),
        rpia_controller(rpia_controller),
        address_display(address)
    {
    rpia_controller->logReceived(address_display,
                            [this](std::vector<uint8_t> data)
                            { sendToDisplay(data); });
}

void DistanceSystemRPiB::sendToDisplay(std::vector<uint8_t> data){
    printf("Ga versturen\n");
    wemos_controller->transmitData(address_display, data);
}