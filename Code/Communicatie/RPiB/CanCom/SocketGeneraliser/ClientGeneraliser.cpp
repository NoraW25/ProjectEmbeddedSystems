#include "ClientGeneraliser.h"

ClientGeneraliser::ClientGeneraliser(std::string ip, bool wemos):
    socket(ClientSocket(ip, wemos)){
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