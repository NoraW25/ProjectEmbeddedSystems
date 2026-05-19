
#ifndef CANRECEIVER_H
#define CANRECEIVER_H

#include "Receiver.h"

class CanReceiver : public Communication::Receiver{
public:
    CanReceiver() = default;
    virtual ~CanReceiver() = default;

    virtual void receive(int*, std::vector<uint8_t>*);
};


#endif