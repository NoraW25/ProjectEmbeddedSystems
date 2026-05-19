
#ifndef CANTRANSMITTER_H
#define CANTRANSMITTER_H

#include "Transmitter.h"
#include "CanSocket.h"

class CanTransmitter : public Communication::Transmitter{
public:
    CanTransmitter();
    virtual ~CanTransmitter() = default;

    virtual void sendData(int, std::vector<uint8_t>) override;
    virtual void sendRequest(int) override;

private:
    CanSocket* socket;
};

#endif CANTRANSMITTER_H