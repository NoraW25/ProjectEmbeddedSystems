#include "ClientSocket.h"

int main(int argc, char const* argv[])
{
    ClientSocket socket;

    while (true){
        if(heeftOntvangen()){
            std::cout<<ontvangst()<<std::endline;
            std::cout<<"Ontvangen"<<std::endline;
        }
    }
}