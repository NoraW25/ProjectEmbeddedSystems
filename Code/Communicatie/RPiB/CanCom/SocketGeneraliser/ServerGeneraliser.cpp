
#include "ServerGeneraliser.h"

ServerGeneraliser::ServerGeneraliser():
    socket(ServerSocket::instance()){

}

ServerGeneraliser::ServerGeneraliser(int port):
    socket(ServerSocket::instance(port)){

}

bool ServerGeneraliser::receive(int* address, std::vector<uint8_t>* data){
    return socket->received(address, data);
}

void ServerGeneraliser::sendData(int address, std::vector<uint8_t> data){
    socket->send(address, data);
}

void ServerGeneraliser::sendRequest(int address){
    std::vector<uint8_t> data;
    socket->send(address, data);
}