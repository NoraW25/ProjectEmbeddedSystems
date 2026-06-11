
#ifndef LIGHTING_H
#define LIGHTING_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "CommunicationController.h"

class LightingSystem {
public:
    LightingSystem(int, std::shared_ptr<Communication::CommunicationController>);

    virtual ~LightingSystem() = default;

    virtual void changeColour(std::vector<uint8_t>);
    virtual void toggleLight(std::vector<uint8_t>);
    virtual void sendNewColour();
    // virtual void addLamp();

private:
    std::map<std::string, int>::iterator current_colour;
    std::shared_ptr<Communication::CommunicationController> controller;
    std::map<std::string, int> colours;

    int button_address;
    int lights_off_address;
    bool lights_on;
};

#endif