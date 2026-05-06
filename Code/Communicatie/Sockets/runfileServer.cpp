#include "ServerSocket.h"

int main(int argc, char const* argv[])
{
    ServerSocket* socket = ServerSocket::instantie();

    while (true){
        if(socket->heeftOntvangen()){
            std::cout<<socket->ontvangst()<<std::endline;
            std::cout<<"Ontvangen"<<std::endline;
        }
    }
}