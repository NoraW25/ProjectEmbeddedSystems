#include "UpdateEvent.h"

#include "SimpleConnection.h"

namespace Events {
	UpdateEvent::UpdateEvent() {

	}

	void UpdateEvent::callConnected() {
		for (int i = 0; i < connections.size(); i++) {
			SimpleConnection* connection = dynamic_cast<SimpleConnection*>(connections[i]);
			connection->getFunction()();
		}
	}

	Connection* UpdateEvent::Connect(std::function<void()> connectFunc) {
		Connection* newConnection = new SimpleConnection(this, connectFunc);
		connections.push_back(newConnection);
		return newConnection;
	}
}