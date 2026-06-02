
#ifndef CANSOCKET_H
#define CANSOCKET_H

#include "Socket.h"
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <vector>

class CanSocket : public Socket {
public:
    virtual ~CanSocket();

    virtual void send(int, std::vector<uint8_t>) override;  
    virtual bool received(int*, std::vector<uint8_t>*) override;
    virtual bool canSend() override;

    static CanSocket* instance();
    static CanSocket* instance(std::string);

private:
    CanSocket();
    CanSocket(std::string);

    void canStartup();
    static CanSocket* pointer_instance;

    struct sockaddr_can socket_address;
    const char* ifname /*default = "can0"*/;

    int can_fd;
    int status;

    struct can_frame received_frame;
    struct can_frame send_frame;

    bool sendOnCan();
};

#endif