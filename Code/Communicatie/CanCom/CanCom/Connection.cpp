#include "Connection.h"

#include "Event.h"

namespace Events {

	Connection::Connection(Event* newEvent) {
		event = newEvent;
	}

	void Connection::Disconnect() {
		event->removeConnection(this);
		delete this;
	}

}