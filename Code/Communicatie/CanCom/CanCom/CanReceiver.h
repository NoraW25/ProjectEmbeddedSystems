
#ifndef CANRECEIVER_H
#define CANRECEIVER_H

#include "Receiver.h"
#include "CanSocket.h"


class CanReceiver : public Communication::Receiver{
public:
    CanReceiver();
    virtual ~CanReceiver() = default;

    virtual void receive(int*, std::vector<uint8_t>*);

private:
    CanSocket* socket;
    int address_buffer;
    std::vector<uint8_t> data_buffer;

    const std::string key_id /*ID:*/;
    const std::string key_data /*DATA:*/;

    int parseId(const std::string&);
    std::vector<uint8_t> parseData(const std::string&);
};


#endif