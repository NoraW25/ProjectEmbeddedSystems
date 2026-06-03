#pragma once
#include "Event.h"

#include <functional>
#include <chrono>

namespace Events {
	class TimedEvent : public Event
	{
	private:
		uint32_t time;
		std::chrono::time_point<std::chrono::steady_clock> startTime;
	public:
		TimedEvent(uint32_t);
		void callConnected();
		Connection* Connect(std::function<void()>);
		void checkTimer();
	};

}