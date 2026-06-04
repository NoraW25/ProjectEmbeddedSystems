#pragma once

#include <vector>
#include <stdint.h>
#include <functional>

namespace Events {
	class RequestorEvent;
	class Connection;
	class Event;
}

namespace Communication {
	class Transmitter;

	class Requestor
	{
	private:
		std::vector<Events::RequestorEvent*> events;
		Transmitter* transmitter;
	public:
		Requestor(Transmitter* transmitter);
		~Requestor();
		Events::Connection* requestData(int address, std::function<void(std::vector<uint8_t>)>);
		bool processData(int address, std::vector<uint8_t> data);
		void removeEvent(Events::Event* event);
	};
}
