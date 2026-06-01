#include "ReceiveBinderEvent.h"

#include "DataConnection.h"

namespace Events {

	ReceiveBinderEvent::ReceiveBinderEvent(Communication::ReceiveBinder* req, int add) : receiveBinder(req), listeningAddress(add) {

	}

	void ReceiveBinderEvent::callConnected() {
		std::vector<uint8_t>* data = &dataBuffer[0];
		for (int i = 0; i < connections.size(); i++) {
			DataConnection* connection = dynamic_cast<DataConnection*>(connections[i]);
			connection->getFunction()(*data);
		}
		dataBuffer.erase(dataBuffer.begin());
		resetFlag();
	}

	void ReceiveBinderEvent::fire(std::vector<uint8_t> data) {
		dataBuffer.push_back(data);
		setFlag();
	}

	int ReceiveBinderEvent::getAddress() {
		return listeningAddress;
	}

	Connection* ReceiveBinderEvent::connect(std::function<void(std::vector<uint8_t>)> func) {
		Connection* newConnection = new DataConnection(this, func);
		connections.push_back(newConnection);
		return newConnection;
	}

}