
#ifndef CLIMATEDISPLAY_H
#define CLIMATEDISPLAY_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "CommunicationController.h"

class ClimateDisplay{
public:
    ClimateDisplay(std::shared_ptr<Communication::CommunicationController>, 
        std::shared_ptr<Communication::CommunicationController>,
        int, int);

    virtual ~ClimateDisplay() = default;

    virtual void displayValue(std::vector<uint8_t>);

private:
    int address_from;
    int address_10led;

    std::shared_ptr<Communication::CommunicationController> controller_can;
    std::shared_ptr<Communication::CommunicationController> controller_rpib;

};

#endif