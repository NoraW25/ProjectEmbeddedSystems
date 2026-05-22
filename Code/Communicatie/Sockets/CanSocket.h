
#ifndef CANSOCKET_H
#define CANSOCKET_H

#include "InterfaceSocket.h"
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <vector>

class CanSocket : public InterfaceSocket {
public:
    virtual ~CanSocket();

    virtual void sendSocket(std::string) override;  
    virtual std::string received() override;
    virtual bool canSend() override;
    virtual bool hasReceived() override;

    void setFrameCan(int address, int dlc, int data);

    static CanSocket* instance();

private:
    CanSocket();

    const char* ifname /*= "can0"*/;
    struct can_frame received_frame;
    struct can_frame send_frame;

    int can_fd;
    int status;

    static CanSocket* pointer_instance;
    struct sockaddr_can socket_address;

    const std::string key_id /*ID:*/;
    const std::string key_dlc /*DLC:*/;
    const std::string key_data /*DATA:*/;

    int parseId(const std::string&);
    int parseDlc(const std::string&);
    std::vector<uint8_t> parseData(const std::string&);

    std::vector<uint8_t> bufferReceivedBytes;
    int bufferReceivedAddress;

    bool send_on_can();

};

#endif