
#ifndef CANSOCKET_H
#define CANSOCKET_H

#include "InterfaceSocket.h"
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>

class CanSocket : public InterfaceSocket {
public:
    virtual ~CanSocket() = default;
    virtual void sendSocket(std::string) override;  
    virtual std::string received() override;
    virtual bool canSend() override;
    virtual bool hasReceived() override;

    static CanSocket* instance();

private:
    CanSocket();

    const char* ifname /*= "can0"*/;
    struct can_frame frame;
    int status;

    int can_fd;

    static CanSocket* pointer_instance;
    struct sockaddr_can socket_address;

};

#endif CANSOCKET_H