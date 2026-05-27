#pragma once

#include "Event.h"
#include "DataConnection.h"
#include <vector>
#include <stdint.h>
#include <functional>

namespace Communication {
	class ReceiveBinder;
}

namespace Events {

	class ReceiveBinderEvent : public Event {
	private:
		Communication::ReceiveBinder* receiveBinder;
		std::vector< std::vector<uint8_t> > dataBuffer;

		int listeningAddress;
	public:
		ReceiveBinderEvent(Communication::ReceiveBinder*, int address);
		void callConnected();
		void fire(std::vector<uint8_t>);
		Connection* connect(std::function<void(std::vector<uint8_t>)>);

		int getAddress();
	};

}