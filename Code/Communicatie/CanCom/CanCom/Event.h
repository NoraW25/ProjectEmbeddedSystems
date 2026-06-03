#pragma once

#include <vector>

namespace Events {
	class Connection;

	class Event {
	protected:
		std::vector<Connection*> connections;
	private:
		bool flag;
	public:
		Event();
		~Event();

		void setFlag();
		void resetFlag();
		bool getFlag();

		virtual void callConnected() = 0;

		virtual void removeConnection(Connection*);
	};

}