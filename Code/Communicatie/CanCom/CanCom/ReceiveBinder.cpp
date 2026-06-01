#include "ReceiveBinder.h"

#include "ReceiveBinderEvent.h"

#include <algorithm>

namespace Communication {
	ReceiveBinder::ReceiveBinder() {

	}

	ReceiveBinder::~ReceiveBinder() {

	}

	Events::Connection* ReceiveBinder::logReceived(int address, std::function<void(std::vector<uint8_t>)> bindFunction) {
		//Find existing request first
		for (int i = 0; i < events.size(); i++) {
			Events::ReceiveBinderEvent* event = events[i];
			if (event->getAddress() == address) {
				return event->connect(bindFunction);
			}
		}
		//Make new event if none has been found already
		Events::ReceiveBinderEvent* event = new Events::ReceiveBinderEvent(this, address);
		events.push_back(event);
		return event->connect(bindFunction);
	}

	void ReceiveBinder::processData(int address, std::vector<uint8_t> data) {
		for (int i = 0; i < events.size(); i++) {
			Events::ReceiveBinderEvent* event = events[i];
			if (event->getAddress() == address) {
				event->fire(data);
			}
		}
	}

	void ReceiveBinder::removeEvent(Events::Event* event) {
		events.erase(
			std::remove(events.begin(), events.end(), event),
			events.end()
		);
	}
}