
#ifndef CLOCK_H
#define CLOCK_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "CommunicationController.h"

class Clock {
public:
    Clock(int, std::shared_ptr<Communication::CommunicationController>);
    virtual ~Clock() = default;

    void updateClock();

private:
    int address_clock;
    std::shared_ptr<Communication::CommunicationController> controller;
    Scheduling::RunServiceController* runservice;
};

#endif