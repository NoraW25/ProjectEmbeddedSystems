// CanCom.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <vector>
#include <stdint.h>

#include "Scheduler.h"
#include "RunServiceController.h"

#include "VirtualReceiver.h"
#include "VirtualTransmitter.h"

#include "CommunicationController.h"
#include "Connection.h"

#include "LightingSystem.h"
#include "Clock.h"
#include "AlarmSystem.h"
#include "DistanceSystem.h"
#include "ClimateDisplay.h"

#include "SocketGeneraliser/CanGeneraliser.h"
#include "SocketGeneraliser/ServerGeneraliser.h"


int main()
{
    Scheduling::Scheduler scheduler;
    Scheduling::RunServiceController* runService = Scheduling::RunServiceController::getInstance(&scheduler);
    scheduler.initEvent();

    CanGeneraliser generaliser;
    ServerGeneraliser generaliser_server;

    // De controllers aanmaken en binden aan de controller
    std::shared_ptr<Communication::CommunicationController> can_controller = 
        std::make_shared<Communication::CommunicationController>(& generaliser, & generaliser);
    std::shared_ptr<Communication::CommunicationController> server_controller = 
        std::make_shared<Communication::CommunicationController>(& generaliser_server, & generaliser_server);


    // Aanmaken van een klasses die een processen aansturen
    LightingSystem lighting_system(500, 430, can_controller);
    Clock clock_systeem(410, can_controller);
    AlarmSystem alarm_system(can_controller, server_controller, 420);
    DistanceSystem distance_system(can_controller, server_controller);
    ClimateDisplay climate_system(can_controller, server_controller, 150, 230);


    while (1) {
        scheduler.update();
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
