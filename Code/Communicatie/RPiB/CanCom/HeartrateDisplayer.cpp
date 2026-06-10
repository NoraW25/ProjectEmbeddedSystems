#include "HeartrateDisplayer.h"
#include "CommunicationController.h"
#include "Connection.h"

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
    if (!displayController)
        return;
    displayController->transmitData(710, data);
}
