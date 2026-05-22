
/*!
 * @file runfileClient.cpp
 * @test Test de werking van de klassen Server- en ClientSocket
 * @brief Maakt een client socket aan en kan daardoor berichten ontvangen en versturen.
 * 
 * Dit is het file dat op de aangewezen client moet draaien.
 * 
 * Hierin wordt een ClientSocket gemaakt. Deze verstuurd indien mogelijk dan meteen bericht naar de server en gaat daarna luisteren.
 * Als er iets ontvangen is, wordt dat uitgelezen. 
 * Vervolgens zal het systeem voor de test 10 seconden wachten tot het nog een bericht verzendt, 
 * dit is blocking, dus er wordt tegelijk GEEN informatie van de server ontvangen. 
 */

#include "ClientSocket.h"

#include <stdio.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char const* argv[])
{
    ClientSocket* socket = new ClientSocket("145.52.127.222");
    ClientSocket* socketWemos = new ClientSocket("145.52.222.116");

    socketWemos->sendSocket("hello there! from RPi");
    socket->sendSocket("Hello there! from client");

    while (true){
        if(socket->hasReceived()){
            std::cout<<socket->received()<<std::endl;
            std::cout<<"Ontvangen"<<std::endl;
        }
        
        if(socketWemos->hasReceived()){
            std::cout<<socket->received()<<std::endl;
            std::cout<<"Ontvangen"<<std::endl;
        }

        sleep(10); // wacht 10 seconden om code te testen
        socket->sendSocket("Hello there! from client");
    }
}