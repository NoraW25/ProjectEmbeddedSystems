#include "HeartrateDisplayer.h"
#include "CommunicationController.h"
#include "Connection.h"
#include <cstdio>

HeartrateDisplayer::HeartrateDisplayer(std::shared_ptr<Communication::CommunicationController> displayCtrl,
                                       std::shared_ptr<Communication::CommunicationController> hartslagCtrl)
    : displayController(displayCtrl), hartslagController(hartslagCtrl), connection(nullptr)
{
    if (hartslagController)
    {
        connection = hartslagController->logReceived(810, [this](std::vector<uint8_t> data)
                                                     { this->handleHeart(data); });
    }
}

HeartrateDisplayer::~HeartrateDisplayer()
{
    if (connection)
    {
        connection->Disconnect();
    }
}

void HeartrateDisplayer::handleHeart(std::vector<uint8_t> data)
{
    std::vector<uint8_t> fixed(8, 0);
    size_t offset = 8 - data.size();
    for (size_t i = 0; i < data.size() && i < 8; i++)
    {
        fixed[offset + i] = data[i];
    }
    displayController->transmitData(710, fixed);
}
