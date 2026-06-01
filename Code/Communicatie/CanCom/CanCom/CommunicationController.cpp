#include "CommunicationController.h"

#include "Transmitter.h"

namespace Communication {
	CommunicationController::CommunicationController(Transmitter* trans, Receiver* rec)
		:transmitter(trans),
		requestor(trans),
		binder(),
		processor(rec, &requestor, &binder)
	{
		
	}

	CommunicationController::~CommunicationController() {

	}

	void CommunicationController::transmitData(int address, std::vector<uint8_t> data) {
		transmitter->sendData(address, data);
	}

	Events::Connection* CommunicationController::requestData(int address, std::function<void(std::vector<uint8_t>)> func) {
		return requestor.requestData(address, func);
	}

	Events::Connection* CommunicationController::logReceived(int address, std::function<void(std::vector<uint8_t>)> func) {
		return binder.logReceived(address, func);
	}
}