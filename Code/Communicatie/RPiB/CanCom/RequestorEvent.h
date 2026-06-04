#pragma once

#include "Event.h"
#include "DataConnection.h"
#include <vector>
#include <stdint.h>
#include <functional>

namespace Communication {
	class Requestor;
}

namespace Events {

	class RequestorEvent : public Event {
	private:
		Communication::Requestor* requestor;
		std::vector< std::vector<uint8_t> > dataBuffer;

		int address;
	public:
		RequestorEvent(Communication::Requestor*, int address);
		void callConnected();
		void fire(std::vector<uint8_t>);
		Connection* connect(std::function<void(std::vector<uint8_t>)>);

		int getAddress();
	};

}