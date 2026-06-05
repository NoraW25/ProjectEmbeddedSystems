
#ifndef CLIENTGENERALISER_H
#define CLIENTGENERALISER_H

#include "../Receiver.h"
#include "../Transmitter.h"
#include "../Socket/ClientSocket.h"


class ClientGeneraliser : 
    public Communication::Receiver, public Communication::Transmitter
{
public:
    ClientGeneraliser(std::string);
    virtual ~ClientGeneraliser() = default;

    virtual void sendData(int, std::vector<uint8_t>) override;
    virtual void sendRequest(int) override;
    virtual bool receive(int*, std::vector<uint8_t>*) override;

private:
    ClientSocket socket;    
};


#endif