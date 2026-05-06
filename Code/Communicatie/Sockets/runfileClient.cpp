#include "ClientSocket.h"

#include <stdio.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char const* argv[])
{
    ClientSocket* socket = ClientSocket::instantie();

    socket->versturen("Hello there! from client");

    while (true){
        if(socket->heeftOntvangen()){
            std::cout<<socket->ontvangst()<<std::endl;
            std::cout<<"Ontvangen"<<std::endl;
        }

        sleep(10); // wacht 10 seconden om code te testen
        socket->versturen("Hello there! from client");
    }
}