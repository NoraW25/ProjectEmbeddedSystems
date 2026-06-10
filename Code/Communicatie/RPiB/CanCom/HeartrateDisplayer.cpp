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

    for (size_t i = 0; i < data.size() && i < 8; i++)
    {
        fixed[i] = data[i];
    }

    printRawBytes(fixed);

    displayController->transmitData(710, fixed);
}

void HeartrateDisplayer::printRawBytes(const std::vector<uint8_t> &data)
{
    printf("RAW DATA (%zu bytes): ", data.size());

    for (size_t i = 0; i < data.size(); i++)
    {
        printf("%02X ", data[i]);
    }

    printf("\n");

    printf("AS DEC: ");
    for (size_t i = 0; i < data.size(); i++)
    {
        printf("%u ", data[i]);
    }

    printf("\n");
}
