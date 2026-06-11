#include "HeartrateDisplayer.h"
#include "CommunicationController.h"
#include "Connection.h"
#include <cstdio>

HeartrateDisplayer::HeartrateDisplayer(std::shared_ptr<Communication::CommunicationController> displayCtrl,
                                       std::shared_ptr<Communication::CommunicationController> hartslagCtrl,
                                       std::shared_ptr<Communication::CommunicationController> raspberryPiA)
    : displayController(displayCtrl), hartslagController(hartslagCtrl), rpiAController(raspberryPiA), connection(nullptr)
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

    if (getHeartrate(data) > 60){
        std::vector<uint8_t> dataByte;
        dataByte.push_back(0);

        rpiAController->transmitData(420, dataByte);
    }
}

double HeartrateDisplayer::getHeartrate(std::vector<uint8_t> data){
    uint64_t raw = 0;

    for (size_t i = 0; i < data.size() && i < sizeof(raw); i++) {
        raw |= (uint64_t)data[i] << (8 * i);
    }

    double value;
    std::memcpy(&value, &raw, sizeof(double));

    return value;
}
