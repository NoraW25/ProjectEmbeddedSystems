#include "VirtualReceiver.h"
#include <stdio.h>

VirtualReceiver::VirtualReceiver()
    : addressBuffer(0), flag(false)
{
}

VirtualReceiver::~VirtualReceiver()
{
}

void VirtualReceiver::setBuffer(int address, std::vector<uint8_t> data)
{
    addressBuffer = address;
    dataBuffer = data;
    flag = true;
}

bool VirtualReceiver::receive(int* address, std::vector<uint8_t>* data)
{
    if (!flag)
    {
        return 0;
    }

    if (address)
    {
        *address = addressBuffer;
    }

    if (data)
    {
        *data = dataBuffer;
    }

    flag = false; // consume the buffer

    return 1;
}