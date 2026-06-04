#pragma once

#include <vector>
#include <stdint.h>

namespace Communication {

	class Receiver {
	public:
		Receiver()=default;
		virtual ~Receiver() = default;
		virtual bool receive(int* address, std::vector<uint8_t>* data) = 0;
	};
}