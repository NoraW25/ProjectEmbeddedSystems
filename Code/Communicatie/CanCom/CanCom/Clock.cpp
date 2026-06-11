
#include <ctime>
#include "Clock.h"
#include "RunServiceController.h"


Clock::Clock(int address, std::shared_ptr<Communication::CommunicationController> controller):
    address_clock(address),
    controller(controller){
        runservice = Scheduling::RunServiceController::getInstance(nullptr);
        runservice->createTask([this]() {updateClock();}, 5000);
}

void Clock::updateClock(){

    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);

    uint8_t hour = (uint8_t) local->tm_hour;
    uint8_t minute = (uint8_t) local->tm_min;

    std::vector<uint8_t> data;

    data.push_back(hour);
    data.push_back(minute);


    controller->transmitData(address_clock, data);

    runservice->createTask([this]() {updateClock();}, 5000);
}
