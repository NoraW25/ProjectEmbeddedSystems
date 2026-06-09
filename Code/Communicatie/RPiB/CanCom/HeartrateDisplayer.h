#pragma once

#include <memory>
#include <vector>
#include <stdint.h>

namespace Communication
{
    class CommunicationController;
}

namespace Events
{
    class Connection;
}

class HeartrateDisplayer
{
public:
    HeartrateDisplayer(std::shared_ptr<Communication::CommunicationController> displayCtrl,
                       std::shared_ptr<Communication::CommunicationController> hartslagCtrl);
    ~HeartrateDisplayer();

private:
    std::shared_ptr<Communication::CommunicationController> displayController;
    std::shared_ptr<Communication::CommunicationController> hartslagController;
    Events::Connection *connection = nullptr;

    void handleHeart(std::vector<uint8_t> data);
};
