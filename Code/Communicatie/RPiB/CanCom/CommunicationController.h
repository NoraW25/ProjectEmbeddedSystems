#pragma once
#include "Requestor.h"
#include "DataProcessor.h"
#include "ReceiveBinder.h"

#include <vector>
#include <stdint.h>
#include <functional>

namespace Scheduling {
	class RunServiceController;
}

namespace Events {
	class Connection;
}

namespace Communication {
	class Transmitter;
	class Receiver;

	class CommunicationController
	{
	public:
		CommunicationController(Transmitter* transmitter, Receiver* receiver);
		~CommunicationController();

		void transmitData(int address, std::vector<uint8_t>);
		Events::Connection* requestData(int address, std::function<void(std::vector<uint8_t>)>);
		Events::Connection* logReceived(int address, std::function<void(std::vector<uint8_t>)>);
	private:
		Requestor requestor;
		DataProcessor processor;
		ReceiveBinder binder;

		Transmitter* transmitter;
	};
}