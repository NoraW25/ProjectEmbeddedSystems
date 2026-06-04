#pragma once

#include "Connection.h"
#include <stdint.h>
#include <vector>
#include <functional>

namespace Events {

	class Event;

	class DataConnection : public Connection {
	private:
		std::function<void(std::vector<uint8_t>)> func;
	public:
		DataConnection(Event*, std::function<void(std::vector<uint8_t>)>);
		std::function<void(std::vector<uint8_t>)> getFunction();
	};

}