
#ifndef CANGENERALISER_H
#define CANGENERALISER_H

#include "../Receiver.h"
#include "../Transmitter.h"
#include "../SocketGeneraliser/CanSocket.h"


class CanGeneraliser : 
    public Communication::Receiver, public Communication::Transmitter
{
public:
    CanGeneraliser();
    CanGeneraliser(std::string);
    virtual ~CanGeneraliser() = default;

    virtual void sendData(int, std::vector<uint8_t>) override;
    virtual void sendRequest(int) override;
    virtual bool receive(int*, std::vector<uint8_t>*) override;

private:
    CanSocket* socket;    
};


#endif