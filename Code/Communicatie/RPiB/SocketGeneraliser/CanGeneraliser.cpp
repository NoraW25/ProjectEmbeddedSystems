
#include "CanGeneraliser.h"

CanGeneraliser::CanGeneraliser():
    socket(CanSocket::instance()){

}

CanGeneraliser::CanGeneraliser(std::string ifname):
    socket(CanSocket::instance(ifname)){

}

bool CanGeneraliser::receive(int* address, std::vector<uint8_t>* data){
    return socket->received(address, data);
}

void CanGeneraliser::sendData(int address, std::vector<uint8_t> data){
    socket->send(address, data);
}

void CanGeneraliser::sendRequest(int address){
    std::vector<uint8_t> data;
    socket->send(address, data);
}