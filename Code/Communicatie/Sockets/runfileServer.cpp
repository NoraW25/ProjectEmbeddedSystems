#include "ServerSocket.h"

#include <stdio.h>
#include <iostream>

int main(int argc, char const* argv[])
{
    ServerSocket* socket = ServerSocket::instantie();

    while (true){
        if(socket->heeftOntvangen()){
            std::cout<<socket->ontvangst()<<std::endl;
            std::cout<<"Ontvangen"<<std::endl;
            socket->versturen("Hello there! from server");
        }
    }
}