#pragma once
#include "Connection.h"
#include <functional>

namespace Events {
	class Event;

	class SimpleConnection: public Connection
	{
	private:
		std::function<void()> func;
	public:
		SimpleConnection(Event*, std::function<void()>);
		std::function<void()> getFunction();
	};
}