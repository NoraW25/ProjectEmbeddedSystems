#pragma once

#include "Receiver.h"

class VirtualReceiver: public Communication::Receiver
{
public:
	VirtualReceiver();
	~VirtualReceiver();
	bool receive(int* address, std::vector<uint8_t>* data);
	void setBuffer(int address, std::vector<uint8_t> data);
private:
	int addressBuffer;
	std::vector<uint8_t> dataBuffer;
	bool flag;
};