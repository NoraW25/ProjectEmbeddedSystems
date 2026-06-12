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

#include "SocketGeneraliser/ClientGeneraliser.h"

#include "ClimateSystem.h"
#include "HeartrateDisplayer.h"
#include "DistanceSystemRPiB.h"

#define BUZZERADDRESS 420
#define LEDBARADDRESS 720


int main()
{
    // Aanmaken van de planner
    Scheduling::Scheduler scheduler;
    Scheduling::RunServiceController *runService = Scheduling::RunServiceController::getInstance(&scheduler);
    scheduler.initEvent();

    // De sockets aanmaken
    ClientGeneraliser generaliser_client_rpia("192.168.0.2", false);
    ClientGeneraliser generaliser_client_wemos_klimaat("192.168.0.101", true);
    ClientGeneraliser generaliser_client_wemos_display("192.168.0.103", true);
    //ClientGeneraliser generaliser_client_wemos_hartslag("192.168.0.102", true);
    ClientGeneraliser generaliser_client_wemos_encoder("192.168.0.104", true);

    // De controllers aanmaken en binden aan de controller
    std::shared_ptr<Communication::CommunicationController> client_controller_rpia =
        std::make_shared<Communication::CommunicationController>(
            &generaliser_client_rpia,
            &generaliser_client_rpia);

    std::shared_ptr<Communication::CommunicationController> client_controller_wemos_klimaat =
        std::make_shared<Communication::CommunicationController>(
            &generaliser_client_wemos_klimaat,
            &generaliser_client_wemos_klimaat);

    std::shared_ptr<Communication::CommunicationController> client_controller_wemos_display =
        std::make_shared<Communication::CommunicationController>(
            &generaliser_client_wemos_display,
            &generaliser_client_wemos_display);

    // std::shared_ptr<Communication::CommunicationController> client_controller_wemos_hartslag =
    //     std::make_shared<Communication::CommunicationController>(
    //         &generaliser_client_wemos_hartslag,
    //         &generaliser_client_wemos_hartslag);

    std::shared_ptr<Communication::CommunicationController> client_controller_wemos_encoder =
        std::make_shared<Communication::CommunicationController>(
            &generaliser_client_wemos_encoder,
            &generaliser_client_wemos_encoder);

    // Aanmaken van een klasses die een processen aansturen
    ClimateSystem climate_system(client_controller_wemos_klimaat, client_controller_rpia, BUZZERADDRESS);
    DistanceSystemRPiB distance_system(client_controller_rpia, client_controller_wemos_display, client_controller_wemos_encoder, LEDBARADDRESS);
    // std::shared_ptr<HeartrateDisplayer> heartrateDisplayer = std::make_shared<HeartrateDisplayer>(
    //     client_controller_wemos_display, client_controller_wemos_hartslag, client_controller_rpia);


    while (1)
    {
        // Kijken of er een nieuwe opdracht klaar staat om uitgevoerd te worden
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
