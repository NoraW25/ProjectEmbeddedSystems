
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

    std::string tcpBuffer;

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

            if (chunk.empty()) {
                continue;
            }
            unsigned char byteValue = chunk[0];
            int value = static_cast<int>(byteValue);

            value = 50;

            std::cout << "Parsed integer: " << value << std::endl;
            std::string text = "ID:410;DLC:1;DATA:" + std::to_string(value) + ";";

            socketCan->sendSocket(text);
            std::cout << "CAN TX: " << text << std::endl;
            std::string text2 ="cansend can0 19a#" + to_string(value);
            system(text2);
        }
    }
}