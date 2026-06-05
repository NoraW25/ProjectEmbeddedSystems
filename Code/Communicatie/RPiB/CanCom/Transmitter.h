#pragma once

#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <vector>
#include <stdint.h>

namespace Communication {
	class Transmitter {
	public:
		Transmitter() = default;
		~Transmitter() = default;
		virtual void sendData(int address, std::vector<uint8_t> data) = 0;
		virtual void sendRequest(int address) = 0;
	};
}

#endif