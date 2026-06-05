#include "Requestor.h"

#include "RequestorEvent.h"
#include "Transmitter.h"

#include <algorithm>

namespace Communication {
	Requestor::Requestor(Transmitter* trans):transmitter(trans) {

	}

	Requestor::~Requestor() {

	}

	Events::Connection* Requestor::requestData(int address, std::function<void(std::vector<uint8_t>)> bindFunction) {
		Events::RequestorEvent* event = new Events::RequestorEvent(this, address);
		events.push_back(event);
		Events::Connection* connection = event->connect(bindFunction);
		
		transmitter->sendRequest(address);

		return connection;
	}

	bool Requestor::processData(int address, std::vector<uint8_t> data) {
		for (int i = 0; i < events.size(); i++) {
			Events::RequestorEvent* event = events[i];
			if (event->getAddress() == address) {
				event->fire(data);
				events.erase(
					std::remove(events.begin(), events.end(), event),
					events.end()
				);
				return true;
			}
		}
		return false;
	}

	void Requestor::removeEvent(Events::Event* event) {

	}
}