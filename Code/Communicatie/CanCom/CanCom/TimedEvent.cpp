#include "TimedEvent.h"

#include "SimpleConnection.h"

namespace Events {
	TimedEvent::TimedEvent(uint32_t setTime) :
		time(setTime),
		startTime(std::chrono::steady_clock::time_point{}) 
	{
		startTime = std::chrono::steady_clock::now();
	}

	void TimedEvent::callConnected() {
		for (int i = 0; i < connections.size(); i++) {
			SimpleConnection* connection = dynamic_cast<SimpleConnection*>(connections[i]);
			connection->getFunction()();
		}
		delete this;
	}

	Connection* TimedEvent::Connect(std::function<void()> connectFunc) {
		Connection* newConnection = new SimpleConnection(this, connectFunc);
		connections.push_back(newConnection);
		return newConnection;
	}

	void TimedEvent::checkTimer() {
		auto now = std::chrono::steady_clock::now();

		auto elapsed =
			std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

		if (elapsed >= time) {
			this->setFlag();
		}
		return;
	}
}