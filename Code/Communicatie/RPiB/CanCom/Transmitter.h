#pragma once

#include <vector>
#include <stdint.h>

namespace Klimaatsysteem {
	class Transmitter {
	public:
		Transmitter() = default;
		~Transmitter() = default;
		virtual void sendData(int address, std::vector<uint8_t> data) = 0;
		virtual void sendRequest(int address) = 0;
	};
}