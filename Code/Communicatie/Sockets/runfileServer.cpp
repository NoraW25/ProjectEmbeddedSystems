
/*!
 * @file runfileServer.cpp
 * @test Test de werking van de klassen Server- en ClientSocket
 * @brief Maakt een server socket aan en kan daardoor berichten ontvangen en versturen.
 * 
 * Dit is het file dat op de aangewezen server moet draaien.
 * 
 * Hierin wordt een ServerSocket gemaakt. Deze kijkt vervolgens in een loop of er een bericht binnenkomt van de client socket.
 * Als er een bericht binnenkomt, zal de serversocket een bericht terugzenden.
 * 
 */

#include "ServerSocket.h"
#include "CanSocket.h"

#include <stdio.h>
#include <iostream>

int main(int argc, char const* argv[])
{
    ServerSocket* socket = ServerSocket::instance();
    //std::cout<<"Na socket Server instantiatie";
    CanSocket* socketCan = CanSocket::instance();
    //std::cout<<"Na socket Can instantiatie";


    while (true){
        if(socketCan->hasReceived()){
            std::cout<<socketCan->received()<<std::endl;
            std::cout<<"Ontvangen"<<std::endl;
            //socketCan->sendSocket("Hello there! from server");
        }

        //socket->sendSocket("Hello there! from server");

        if (socket->hasReceived()) {

            std::string chunk = socket->received();
            std::cout << "TCP CHUNK=[" << chunk << "]" << std::endl;

            tcpBuffer += chunk;   // voeg toe aan buffer

            // -----------------------------
            // 3. Verwerk complete berichten
            // -----------------------------
            size_t position;
            while ((position = tcpBuffer.find(";")) != std::string::npos) {

                std::string message = tcpBuffer.substr(0, position);
                tcpBuffer.erase(0, position + 1);

                std::cout << "TCP MESSAGE=[" << message << "]" << std::endl;

                // -----------------------------
                // 4. Integer uit bericht halen
                // -----------------------------
                int value = 0;
                if (sscanf(message.c_str(), "%*[^0-9]%d", &value) == 1) {

                    std::cout << "Parsed integer: " << value << std::endl;

                    // -----------------------------
                    // 5. CAN bericht sturen
                    // -----------------------------
                    std::string text =
                        "ID:410;DCL:1;Data:" + std::to_string(value) + ";";

                    socketCan->sendSocket(text);
                    std::cout << "CAN TX: " << text << std::endl;

                } else {
                    std::cout << "Kon geen integer uit bericht halen" << std::endl;
                }
            }

    }
}