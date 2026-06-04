#pragma once

#include <vector>
#include <stdint.h>
#include <functional>

namespace Events {
	class ReceiveBinderEvent;
	class Connection;
	class Event;
}

namespace Communication {
	class ReceiveBinder
	{
	private:
		std::vector<Events::ReceiveBinderEvent*> events;
	public:
		ReceiveBinder();
		~ReceiveBinder();
		Events::Connection* logReceived(int address, std::function<void(std::vector<uint8_t>)>);
		void processData(int address, std::vector<uint8_t> data);
		void removeEvent(Events::Event* event);
	};
}
