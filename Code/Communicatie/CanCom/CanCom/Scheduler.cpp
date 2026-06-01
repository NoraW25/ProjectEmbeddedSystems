#include "Scheduler.h"

#include "Event.h"

namespace Scheduling {
	Scheduler::Scheduler():updateEvent(nullptr) {

	}

	Scheduler::~Scheduler() {

	}

	void Scheduler::update() {
		updateEvent->callConnected();
		for (int i = 0; i < events.size(); i++) {
			if (events[i]->getFlag()) {
				events[i]->callConnected();
			}
		}
	}

	void Scheduler::addEvent(Events::Event* event) {
		events.push_back(event);
	}

	void Scheduler::removeEvent(Events::Event* event) {
		events.erase(
			std::remove(events.begin(), events.end(), event),
			events.end()
		);
	}

	Events::Connection* Scheduler::connectToUpdate(std::function<void()> func) {
		return updateEvent->Connect(func);
	}

	void Scheduler::initEvent() {
		updateEvent = new Events::UpdateEvent;
	}
}