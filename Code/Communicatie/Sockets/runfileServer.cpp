
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

        if(socket->hasReceived()){
            std::string waarde = socket->received();
            std::cout<<waarde<<std::endl;
            std::cout<<"Ontvangen"<<std::endl;
            std::string text = "ID:410;DCL:1;Data:" + waarde + ";";
            socketCan->sendSocket(text);
        }
    }
}