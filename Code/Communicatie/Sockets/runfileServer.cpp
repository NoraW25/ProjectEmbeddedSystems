#include "ServerSocket.h"

int main(int argc, char const* argv[])
{
    ServerSocket socket;

    while (true){
        if(heeftOntvangen()){
            std::cout<<ontvangst()<<std::endline;
            std::cout<<"Ontvangen"<<std::endline;
        }
    }
}