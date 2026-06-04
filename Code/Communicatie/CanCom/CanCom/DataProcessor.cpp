#include "DataProcessor.h"

#include "Connection.h"
#include "ReceiveBinder.h"
#include "Requestor.h"
#include "RunServiceController.h"

#include <stdio.h>

namespace Communication {
	DataProcessor::DataProcessor(Receiver* newRec, Requestor* req, ReceiveBinder* bind) : receiver(newRec), requestor(req), binder(bind) {
		Scheduling::RunServiceController* runService = Scheduling::RunServiceController::getInstance(nullptr);
		eventConnection = runService->connectToUpdate(
			[this]() { this->processData(); }
		);
	}

	void DataProcessor::processData() {
		printf("processingData");
		int recAddress = 0;
		std::vector<uint8_t> data;

		if (receiver->receive(&recAddress, &data) == false) {
			return;
		}
		printf("%d ontvangen", recAddress);
		if (requestor->processData(recAddress, data)) {
			printf("data has been processed beforehand\n");
			return;
		}

		binder->processData(recAddress, data);
	}
}