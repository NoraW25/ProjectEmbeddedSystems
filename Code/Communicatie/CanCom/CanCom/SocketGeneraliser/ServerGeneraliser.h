
#ifndef SERVERGENERALISER_H
#define SERVERGENERALISER_H

#include "../Receiver.h"
#include "../Transmitter.h"
#include "../Socket/ServerSocket.h"


class ServerGeneraliser : 
    public Communication::Receiver, public Communication::Transmitter
{
public:
    ServerGeneraliser();
    ServerGeneraliser(int);
    virtual ~ServerGeneraliser() = default;

    virtual void sendData(int, std::vector<uint8_t>) override;
    virtual void sendRequest(int) override;
    virtual bool receive(int*, std::vector<uint8_t>*) override;

private:
    ServerSocket* socket;    
};


#endif