
#include "CanReceiver.h"

CanReceiver::CanReceiver():
    socket(CanSocket::instance()),
    key_id("ID:"),
    key_data("DATA:"){

}

bool CanReceiver::receive(int* address, std::vector<uint8_t>* data){
    if (socket->hasReceived() == false) {
        return false;
    }
    std::string message = socket->received();

    if (message.length() == 0){
        std::cout<<"ERROR: CanReceiver, leeg bericht gekregen"<<std::endl;
        return;
    }

    address_buffer = parseId(message);
    data_buffer = parseData(message);

    address = &address_buffer;
    data = &data_buffer;
    return true;
}

int CanSocket::parseId(const std::string& message){
    size_t key = message.find(key_id) + key_id.length();
    size_t value = message.find(";", key);
    int id = std::stoi(message.substr(key, value - key));
    return id;
}

std::vector<uint8_t> CanSocket::parseData(const std::string& message){
    std::vector<uint8_t> result;

    size_t position = message.find(key_data) + key_data.length();
    size_t next = message.find(";", position);

    while(next != std::string::npos){
        int value = std::stoi(message.substr(pos, next-pos));
        result.push_back((uint8_t) value);

        position = next + 1;
        next = message.find(";", position);
    }

    return result;
}