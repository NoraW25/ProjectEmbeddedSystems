#pragma once

#include "Receiver.h"

namespace Events {
	class Connection;
}

namespace Communication {
	class Requestor;
	class ReceiveBinder;

	class DataProcessor
	{
	public:
		DataProcessor(Receiver*, Requestor*, ReceiveBinder*);
		void processData();
	private:
		Events::Connection* eventConnection;
		Receiver* receiver;
		Requestor* requestor;
		ReceiveBinder* binder;
	};

}