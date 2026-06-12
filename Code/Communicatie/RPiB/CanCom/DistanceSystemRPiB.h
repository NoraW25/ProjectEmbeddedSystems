
#ifndef DISTANCESYSTEMRPIB_H
#define DISTANCESYSTEMRPIB_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "CommunicationController.h"

class DistanceSystemRPiB {
public:
    DistanceSystemRPiB(std::shared_ptr<Communication::CommunicationController>, 
        std::shared_ptr<Communication::CommunicationController>, 
        std::shared_ptr<Communication::CommunicationController>,
        int);

    virtual ~DistanceSystemRPiB() = default;

    virtual void sendToDisplay(std::vector<uint8_t>);
    virtual void sendDistance(std::vector<uint8_t>);

private:
    std::shared_ptr<Communication::CommunicationController> wemos_controller;
    std::shared_ptr<Communication::CommunicationController> wemos_controller_encoder;
    std::shared_ptr<Communication::CommunicationController> rpia_controller;
    int address_display;
    int address_distance_wemos;
    int address_distance_rpia;

};

#endif