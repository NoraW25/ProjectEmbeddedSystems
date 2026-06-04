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

//Communication::CommunicationController* canController;
Communication::CommunicationController* client_controller;


void testFunction2(std::vector<uint8_t> data) {
    printf("[TestFunction] Data (%zu bytes): ", data.size());
    for (size_t i = 0; i < data.size(); ++i)
    {
        printf("%02X ", data[i]);

    }
    client_controller->transmitData(0x100, data);
}

void basicTestFunction() {
    printf("updated\n");
}

void calculate(std::vector<uint8_t>) {

}

int main()
{
    Scheduling::Scheduler scheduler;
    Scheduling::RunServiceController* runService = Scheduling::RunServiceController::getInstance(&scheduler);
    scheduler.initEvent();

    ClientGeneraliser generaliser_client;
    client_controller = new Communication::CommunicationController(& generaliser_client, & generaliser_client);

    client_controller->logReceived(0x10, *testFunction2);

    std::vector<uint8_t> begin_data;
    begin_data.push_back(45);
    begin_data.push_back(85);


    client_controller->transmitData(0x10, begin_data);

    while (1) {
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

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
