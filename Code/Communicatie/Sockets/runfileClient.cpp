#include "ClientSocket.h"

#include <stdio.h>
#include <iostream>

int main(int argc, char const* argv[])
{
    ClientSocket* socket = ClientSocket::instantie();

    while (true){
        if(socket->heeftOntvangen()){
            std::cout<<socket->ontvangst()<<std::endl;
            std::cout<<"Ontvangen"<<std::endl;
        }
    }
}