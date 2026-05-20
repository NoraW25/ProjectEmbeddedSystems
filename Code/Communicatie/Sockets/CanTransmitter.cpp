
#include "CanTransmitter.h"


CanTransmitter::CanTransmitter():
    socket(CanSocket::instance()),
    nr_bytes(8),
    key_id("ID:"),
    key_dcl("DCL:"),
    key_data("DATA:"){
    
}

void CanTransmitter::sendData(int address, std::vector<uint8_t> data){
    std::string post = "";

    post += key_id + std::to_string(address) + ";" + key_dcl + std::to_string(data.size()) + ";" + key_data;

    for (int i = 0; i < data.size(); i++){
        // Vul met databytes
        post += std::to_string(data[i]);
        post += ";"
    }

    std::cout<<post<<std::endl;

    socket->sendSocket(post);
}

void CanTransmitter::sendRequest(int address){

}
