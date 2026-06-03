#include "RunServiceController.h"

#include "Scheduler.h"

namespace Scheduling {
	RunServiceController::RunServiceController(Scheduler* newsched): scheduler(newsched) {

	}

	RunServiceController* RunServiceController::_instance = 0;

	RunServiceController* RunServiceController::getInstance(Scheduler* newsched) {
		if (_instance == 0) {
			_instance = new RunServiceController(newsched);
		}
		return _instance;
	}

	void RunServiceController::addEvent(Events::Event* event) {
		scheduler->addEvent(event);
	}

	void RunServiceController::removeEvent(Events::Event* event) {
		scheduler->removeEvent(event);
	}

	Events::Connection* RunServiceController::connectToUpdate(std::function<void()> func) {
		return scheduler->connectToUpdate(func);
	}

	Events::Connection* RunServiceController::createTask(std::function<void()> func, uint32_t delay) {
		return scheduler->createTask(func, delay);
	}
}