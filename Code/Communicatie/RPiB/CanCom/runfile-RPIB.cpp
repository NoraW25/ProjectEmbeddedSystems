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

//Tijdelijke definitie voor client controllers, zodat deze in de testfuncties gebruikt kunnen worden
Communication::CommunicationController* client_controller_rpia;
//Communication::CommunicationController* client_controller_wemos;

// Aansturing van het klimaatsysteem
void klimaatfunctie(std::vector<uint8_t> data) {
    // Print eerst de data
    for (size_t i = 0; i < data.size(); ++i)
    {
        printf("%02X ", data[i]);

    }

    // Verzend de data terug naar 
    //client_controller_rpia->transmitData(0x10, data);
}

void basicTestFunction() {
    printf("updated\n");
}




int main()
{
    // Aanmaken van de planner
    Scheduling::Scheduler scheduler;
    Scheduling::RunServiceController* runService = Scheduling::RunServiceController::getInstance(&scheduler);
    scheduler.initEvent();

    // De sockets aanmaken
    ClientGeneraliser generaliser_client_rpia("145.52.127.222");
    ClientGeneraliser generaliser_client_wemos("145.52.127.246");
    // De controllers aanmaken en binden aan de controller
    client_controller_rpia = new Communication::CommunicationController(& generaliser_client_rpia, & generaliser_client_rpia);
    
    std::shared_ptr<Communication::CommunicationController> client_controller_wemos = 
        std::make_shared<Communication::CommunicationController>(&generaliser_client_wemos, &generaliser_client_wemos);


    // Het uitvoeren van taken en functies op bepaalde momenten
    //client_controller_wemos->logReceived(610, *klimaatfunctie);

    // Aanmaken van een klasse die een proces aanstuurd
    ClimateSystem climate_system(client_controller_wemos);


    runService->createTask(*transmitFunc, 640);

    while (1) {
        // printf("updating\n");
        // Kijken of er een nieuwe opdracht klaar staat om uitgevoerd te worden
        scheduler.update();
    }

    /*runService->connectToUpdate(*basicTestFunction);
    scheduler.update();

    std::vector<uint8_t> testData1;
    testData1.push_back(10);
    testData1.push_back(20);
    canController.transmitData(100, testData1);

    canController.requestData(200, *testFunction);
    std::vector<uint8_t> testData2;
    testData2.push_back(60);
    testData2.push_back(44);
    receiver.setBuffer(200, testData2);
    scheduler.update();

    canController.logReceived(300, *testFunction);
    std::vector<uint8_t> testData3;
    testData3.push_back(0x00);
    testData3.push_back(0xAA);
    testData3.push_back(0xFF);
    receiver.setBuffer(300, testData3);
    scheduler.update();*/
}

void transmitFunc() {
    std::vector<uint8_t> begin_data;
    begin_data.push_back(45);
    begin_data.push_back(85);
    begin_data.push_back('\n');
    printf("Transmitfunc: %d\n", begin_data.size());
    client_controller_wemos->transmitData(640, begin_data);
    //client_controller_wemos->transmitData(640, '\n');
    printf("transmit\n");
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
