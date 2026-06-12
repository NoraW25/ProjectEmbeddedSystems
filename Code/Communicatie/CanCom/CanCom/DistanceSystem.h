
#ifndef DISTANCESYSTEM_H
#define DISTANCESYSTEM_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "CommunicationController.h"

class DistanceSystem{
public:
    DistanceSystem(std::shared_ptr<Communication::CommunicationController>, 
        std::shared_ptr<Communication::CommunicationController>);
    DistanceSystem(std::shared_ptr<Communication::CommunicationController>, 
        std::shared_ptr<Communication::CommunicationController>, int, int, int);
    virtual ~DistanceSystem() = default;

    virtual void addToDistance(std::vector<uint8_t>);
    virtual void resetDistance(std::vector<uint8_t>);

    virtual void calculateAmountOfLights();


private:
    int distance;
    int amount_of_lights;
    int finish_distance;
    int amount_of_lights_available;

    int address_display;
    int address_reset_button;
    int address_turns;

    std::shared_ptr<Communication::CommunicationController> can_controller;
    std::shared_ptr<Communication::CommunicationController> rpib_controller;

};

#endif