
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
    int nr_bytes;

    const std::string key_id /*ID:*/;
    const std::string key_dcl /*DCL:*/;
    const std::string key_data /*DATA:*/;
};

#endif CANTRANSMITTER_H