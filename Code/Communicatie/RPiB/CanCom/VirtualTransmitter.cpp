#include "VirtualTransmitter.h"
#include <stdio.h>

VirtualTransmitter::VirtualTransmitter()
{
}

VirtualTransmitter::~VirtualTransmitter()
{
}

void VirtualTransmitter::sendData(int address, std::vector<uint8_t> data)
{
    printf("[VirtualTransmitter] Sending data to address: %d\n", address);

    printf("[VirtualTransmitter] Data (%zu bytes): ", data.size());
    for (size_t i = 0; i < data.size(); ++i)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

void VirtualTransmitter::sendRequest(int address)
{
    printf("[VirtualTransmitter] Sending request to address: %d\n", address);
}