
#include "CanTransmitter.h"


CanTransmitter::CanTransmitter():
    socket(CanSocket::instance()){
    
}

void CanTransmitter::sendData(int address, std::vector<uint8_t> data){
    
    socket->sendSocket();
}

void CanTransmitter::sendRequest(int address){

}
