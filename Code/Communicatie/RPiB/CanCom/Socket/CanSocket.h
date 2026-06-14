
/*!
 * @file CanSocket.h
 * @class CanSocket
 * @brief Maakt een CANbus socket aan, kan daardoor berichten ontvangen en versturen op de CANbus.
 * Met een object van deze klasse, kunnen berichten worden gelezen van de CANbus 
 * en verstuurd worden op de CANbus op een non blocking manier.
 */

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
    /*!
    * @brief Sluit de CanSocket.
    *
    * Als de CanSocket nog actief is, zal deze worden afgesloten.
    */
    virtual ~CanSocket();

    virtual void send(int, std::vector<uint8_t>) override;  
    virtual bool received(int*, std::vector<uint8_t>*) override;
    virtual bool canSend() override;


    /*!
    * @brief Geeft de instance van de CanSocket terug.
    *
    * Deze methode geeft een CanSocket terug, 
    * als deze nog niet bestaat wordt er standaard op interface can0 een socket geprobeert aan te maken.
    * 
    * @return CanSocket* - Een pointer naar de bestaande CanSocket.
    */
    static CanSocket* instance();

    /*!
    * @brief Geeft de instance van de CanSocket terug.
    *
    * Deze methode geeft een CanSocket terug, 
    * als deze nog niet bestaat wordt er op de meegegeven interface een socket geprobeert aan te maken.
    * 
    * @param string - std::string waar een andere interface dan de standaard can0 meegegeven kan worden.
    * 
    * @return CanSocket* - Een pointer naar de bestaande CanSocket.
    */
    static CanSocket* instance(std::string);

private:
    CanSocket();
    CanSocket(std::string);

    void canStartup();
    static CanSocket* pointer_instance;

    struct sockaddr_can socket_address;
    std::string ifname /*default = "can0"*/;

    int can_fd;
    int status;

    struct can_frame received_frame;
    struct can_frame send_frame;

    bool sendOnCan();
};

#endif