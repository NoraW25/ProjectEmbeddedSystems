#pragma once

#include <vector>

#include "UpdateEvent.h"
#include "TimedEvent.h"
#include <functional>
#include <chrono>

namespace Scheduling {
	class Scheduler
	{
	private:
		std::vector<Events::Event*> events;
		std::vector<Events::TimedEvent*> timedEvents;
		Events::UpdateEvent* updateEvent;
		int deletedEvents;
	public:
		Scheduler();
		~Scheduler();
		void update();
		void addEvent(Events::Event*);
		void removeEvent(Events::Event*);
		void removeTimedEvent(Events::TimedEvent*);
		void initEvent();
		Events::Connection* connectToUpdate(std::function<void()>);
		Events::Connection* createTask(std::function<void()>, uint32_t delay);
	};

}
