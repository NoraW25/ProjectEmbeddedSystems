#include "RequestorEvent.h"

#include "DataConnection.h"

namespace Events {

	RequestorEvent::RequestorEvent(Communication::Requestor* req, int add) : requestor(req), address(add) {

	}

	void RequestorEvent::callConnected() {
		std::vector<uint8_t>* data = &dataBuffer[0];
		for (int i = 0; i < connections.size(); i++) {
			DataConnection* connection = dynamic_cast<DataConnection*>(connections[i]);
			connection->getFunction()(*data);
		}
		dataBuffer.erase(dataBuffer.begin());
		delete this;
	}

	void RequestorEvent::fire(std::vector<uint8_t> data) {
		dataBuffer.push_back(data);
		setFlag();
	}

	int RequestorEvent::getAddress() {
		return address;
	}

	Connection* RequestorEvent::connect(std::function<void(std::vector<uint8_t>)> func) {
		Connection* newConnection = new DataConnection(this, func);
		connections.push_back(newConnection);
		return newConnection;
	}

	void RequestorEvent::removeConnection(Connection* removing) {
		int numConnections = connections.size();
		for (int i = 0; i < numConnections; i++) {
			if (connections[i] == removing) {
				connections.erase(connections.begin() + i);
				return;
			}
		}
		if (numConnections <= 1) {
			delete this;
		}
	}

}