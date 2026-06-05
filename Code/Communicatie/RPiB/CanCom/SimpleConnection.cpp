#include "SimpleConnection.h"

namespace Events {

	SimpleConnection::SimpleConnection(Event* newEvent, std::function<void()> newFunc) : Connection(newEvent) {
		func = newFunc;
	}

	std::function<void()> SimpleConnection::getFunction() {
		return func;
	}

}