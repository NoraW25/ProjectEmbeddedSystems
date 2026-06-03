#include "Scheduler.h"

#include "Event.h"
//#include "TimedEvent.h"

#include <algorithm>
#include <stdio.h>

using Clock = std::chrono::steady_clock;

namespace Scheduling {
	Scheduler::Scheduler():updateEvent(nullptr) {
		
	}

	Scheduler::~Scheduler() {

	}

	void Scheduler::update() {
		for (int i = 0; i < timedEvents.size(); i++) {
			timedEvents[i]->checkTimer();
		}

		updateEvent->callConnected();
		deletedEvents = 0;
		int startSize = events.size();
		int i = 0;
		while (i<startSize-deletedEvents) {
			if (events[i]->getFlag()) {
				events[i]->callConnected();
			}
			i++;
		}
	}

	void Scheduler::addEvent(Events::Event* event) {
		events.push_back(event);
	}

	void Scheduler::removeEvent(Events::Event* event) {
		Events::TimedEvent* d = dynamic_cast<Events::TimedEvent*>(event);

		if (d != nullptr) {
			timedEvents.erase(
				std::remove(timedEvents.begin(), timedEvents.end(), d),
				timedEvents.end()
			);
		}

		events.erase(
			std::remove(events.begin(), events.end(), event),
			events.end()
		);
		deletedEvents += 1;

		printf("[Scheduler] removed events, remaining events: %d, remaining timed events: %d", events.size(), timedEvents.size());
	}

	Events::Connection* Scheduler::connectToUpdate(std::function<void()> func) {
		return updateEvent->Connect(func);
	}

	Events::Connection* Scheduler::createTask(std::function<void()> func, uint32_t delay) {
		Events::TimedEvent* newTask = new Events::TimedEvent(delay);
		timedEvents.push_back(newTask);
		return newTask->Connect(func);
	}

	void Scheduler::initEvent() {
		updateEvent = new Events::UpdateEvent;
	}
}