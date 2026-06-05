#include "ClientGeneraliser.h"

ClientGeneraliser::~ClientGeneraliser() = default;


ClientGeneraliser::ClientGeneraliser(std::string ip):
    socket(ClientSocket(ip)){
}

bool ClientGeneraliser::receive(int* address, std::vector<uint8_t>* data){
    return socket.received(address, data);
}

void ClientGeneraliser::sendData(int address, std::vector<uint8_t> data){
    socket.send(address, data);
}

void ClientGeneraliser::sendRequest(int address){
    std::vector<uint8_t> data;
    socket.send(address, data);
}