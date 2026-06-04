#pragma once
#include "Transmitter.h"

class VirtualTransmitter: public Communication::Transmitter
{
public:
	VirtualTransmitter();
	~VirtualTransmitter();
	void sendData(int address, std::vector<uint8_t> data);
	void sendRequest(int address);
};

