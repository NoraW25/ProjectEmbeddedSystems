
#ifndef CLIMATESENSOR_H
#define CLIMATESENSOR_H

#include <memory>
#include <vector>
#include "CommunicationController.h"

class ClimateSystem;

class ClimateSensor {
public:
    ClimateSensor(int, std::shared_ptr<Communication::CommunicationController>, ClimateSystem*);

    virtual ~ClimateSensor() = default;
    virtual void setCurrentValue(std::vector<uint8_t>);

    virtual double getCurrentValue();
    virtual int getStatus();

private:
    uint8_t address;
    double value;
    int status;
    std::shared_ptr<Communication::CommunicationController> controller;
    ClimateSystem* system;		
};


#endif