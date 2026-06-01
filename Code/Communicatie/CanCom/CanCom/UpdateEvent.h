#pragma once

#include "Event.h"
#include <functional>

namespace Events {
	class UpdateEvent: public Event
	{
	public:
		UpdateEvent();
		void callConnected();
		Connection* Connect(std::function<void()>);
	};

}