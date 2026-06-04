#pragma once

#include <vector>

#include "UpdateEvent.h"
#include <functional>

namespace Scheduling {
	class Scheduler
	{
	private:
		std::vector<Events::Event*> events;
		Events::UpdateEvent* updateEvent;
	public:
		Scheduler();
		~Scheduler();
		void update();
		void addEvent(Events::Event*);
		void removeEvent(Events::Event*);
		void initEvent();
		Events::Connection* connectToUpdate(std::function<void()>);
	};

}
