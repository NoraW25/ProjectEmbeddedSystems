#pragma once

namespace Events {

	class Event;

	class Connection
	{
	public:
		Connection(Event*);
		void Disconnect();
	private:
		Event* event;
	protected:
		virtual ~Connection() = default;
	};

}