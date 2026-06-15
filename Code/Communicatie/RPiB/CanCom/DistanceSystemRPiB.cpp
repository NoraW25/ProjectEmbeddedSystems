
#include "DistanceSystemRPiB.h"

DistanceSystemRPiB::DistanceSystemRPiB(std::shared_ptr<Communication::CommunicationController> rpia_controller, 
    std::shared_ptr<Communication::CommunicationController> wemos_controller, 
    std::shared_ptr<Communication::CommunicationController> wemos_controller_encoder, 
    int address):    
        wemos_controller(wemos_controller),
        rpia_controller(rpia_controller),
        wemos_controller_encoder(wemos_controller_encoder),
        address_display(address),
        address_distance_wemos(910),
        address_distance_rpia(512)
    {
    rpia_controller->logReceived(address_display,
                            [this](std::vector<uint8_t> data)
                            { sendToDisplay(data); });

    wemos_controller_encoder->logReceived(address_distance_wemos,
                            [this](std::vector<uint8_t> data)
                            { sendDistance(data); });
}

void DistanceSystemRPiB::sendToDisplay(std::vector<uint8_t> data){
    wemos_controller->transmitData(address_display, data);
}

void DistanceSystemRPiB::sendDistance(std::vector<uint8_t> data){
    rpia_controller->transmitData(address_distance_rpia, data);
}