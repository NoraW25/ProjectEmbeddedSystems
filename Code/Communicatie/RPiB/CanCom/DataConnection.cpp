#include "DataConnection.h"

namespace Events {

	DataConnection::DataConnection(Event* newEvent, std::function<void(std::vector<uint8_t>)> newFunc) : Connection(newEvent) {
		func = newFunc;
	}

	std::function<void(std::vector<uint8_t>)> DataConnection::getFunction() {
		return func;
	}

}