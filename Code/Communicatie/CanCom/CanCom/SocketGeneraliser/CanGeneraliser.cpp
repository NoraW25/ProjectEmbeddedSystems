
#include "CanGeneraliser.h"

CanGeneraliser::CanGeneraliser():
    socket(CanSocket::instance()){

}

bool CanGeneraliser::receive(int* address, std::vector<uint8_t>* data){
    return socket->received(address, data);
}

void CanTransmitter::sendData(int address, std::vector<uint8_t> data){
    socket->send(address, data);
}

void CanTransmitter::sendRequest(int address){
    std::vector<uint8_t> data;
    socket->send(address, data);
}