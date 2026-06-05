#pragma once

#include <functional>
#include <stdint.h>

namespace Events {
	class Event;
	class TimedEvent;
	class Connection;
}

namespace Scheduling {
	class Scheduler;

	class RunServiceController
	{
	public:
		// Access the singleton instance
		static RunServiceController* getInstance(Scheduler*);

		// Delete copy/move semantics
		RunServiceController(const RunServiceController&) = delete;
		RunServiceController& operator=(const RunServiceController&) = delete;
		RunServiceController(RunServiceController&&) = delete;
		RunServiceController& operator=(RunServiceController&&) = delete;

		void addEvent(Events::Event* event);
		void removeEvent(Events::Event* event);
		void removeTimedEvent(Events::TimedEvent*);
		Events::Connection* connectToUpdate(std::function<void()>);
		Events::Connection* createTask(std::function<void()>, uint32_t delay);
	private:
		// Private constructor/destructor
		RunServiceController(Scheduler*);
		~RunServiceController() = default;

		Scheduler* scheduler;

		static RunServiceController* _instance;
	};

}