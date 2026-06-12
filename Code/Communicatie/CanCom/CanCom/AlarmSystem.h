

#ifndef ALARMSYSTEEM_H
#define ALARMSYSTEEM_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "CommunicationController.h"

class AlarmSystem {
public:
    AlarmSystem(std::shared_ptr<Communication::CommunicationController>, std::shared_ptr<Communication::CommunicationController>, int);

    virtual ~AlarmSystem() = default;

    virtual void soundAlarm(std::vector<uint8_t>);

private:
    int alarm_address;
    std::shared_ptr<Communication::CommunicationController> controller_can;
    std::shared_ptr<Communication::CommunicationController> controller_rpib;

};

#endif