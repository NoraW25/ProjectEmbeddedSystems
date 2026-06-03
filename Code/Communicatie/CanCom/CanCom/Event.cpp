#include "Event.h"

#include "Connection.h"
#include "RunServiceController.h"

#include <stdio.h>

namespace Events {

	Event::Event() :flag(false) {
		Scheduling::RunServiceController *runService = Scheduling::RunServiceController::getInstance(nullptr);
		runService->addEvent(this);
	}

	Event::~Event() {
		printf("[Event] event removing\n");
		for (int i = 0; i < connections.size(); i++) {
			connections[i]->Disconnect();
		}
		Scheduling::RunServiceController* runService = Scheduling::RunServiceController::getInstance(nullptr);
		runService->removeEvent(this);
	}

	void Event::setFlag() {
		flag = true;
	}

	void Event::resetFlag() {
		flag = false;
	}

	bool Event::getFlag() {
		return flag;
	}

	void Event::removeConnection(Connection* removing) {
		int numConnections = connections.size();
		for (int i = 0; i < numConnections; i++) {
			if (connections[i] == removing) {
				connections.erase(connections.begin() + i);
				return;
			}
		}
	}

}