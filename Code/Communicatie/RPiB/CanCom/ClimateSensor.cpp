
#include "ClimateSensor.h"

#include "ClimateSystem.h"
#include <climits>
#include <cstring>

ClimateSensor::ClimateSensor(int address, std::shared_ptr<Communication::CommunicationController> controller, ClimateSystem* system):
    address(address),
    value(20.0),
    status(0),
    controller(controller),
    system(system){
    
    controller->logReceived(address, 
        [this](std::vector<uint8_t> data) { setCurrentValue(data); } 
        // Om this af te vangen en alleen het type void (*)(std::vector<uint8_t>) mee te geven ipv void (ClimateSensor::*)(std::vector<uint8_t>)
    ); 
}

void ClimateSensor::setCurrentValue(std::vector<uint8_t> data){
    status = 0;

    uint64_t raw = 0;

    // LSB eerst
    for (int i = 0; i < 8; i++) {
        raw |= (uint64_t)(uint8_t)data[i] << (8 * i);
    }

    std::memcpy(&value, &raw, sizeof(double));

    printf("%d\n", raw);
    printf("%d\n", value);

    system->calculateSettings();
}

double ClimateSensor::getCurrentValue(){
    if (status < INT_MAX){
        status++;
    }
    
    return value;
}

int ClimateSensor::getStatus(){
    return status;
}