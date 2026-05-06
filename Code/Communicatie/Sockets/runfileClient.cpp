#include "ClientSocket.h"

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