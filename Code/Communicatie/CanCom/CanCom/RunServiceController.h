#pragma once

#include <functional>

namespace Events {
	class Event;
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
		Events::Connection* connectToUpdate(std::function<void()>);

	private:
		// Private constructor/destructor
		RunServiceController(Scheduler*);
		~RunServiceController() = default;

		Scheduler* scheduler;

		static RunServiceController* _instance;
	};

}