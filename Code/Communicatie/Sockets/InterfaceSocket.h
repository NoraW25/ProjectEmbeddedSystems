
#ifndef INTERFACESOCKET_H
#define INTERFACESOCKET_H

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

class Bericht;

class InterfaceSocket{
public:
    virtual void versturen(std::string) = 0;  
    virtual std::string ontvangst() = 0;
    virtual bool kanVersturen() = 0;
    virtual bool heeftOntvangen() = 0;
};


#endif