
#ifndef CLIMATESYSTEM_H
#define CLIMATESYSTEM_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "ClimateSensor.h"
#include "ClimateActuatorPWM.h"
#include "..\RPiB\CanCom\CommunicationController.h"

class Communication::CommunicationController;

class ClimateSystem{
public:
    ClimateSystem(std::shared_ptr<Communication::CommunicationController>);

    virtual ~ClimateSystem() = default;

    virtual void calculateSettings();

    virtual void addSensor(std::string, std::shared_ptr<ClimateSensor>);
    virtual void addActuatorPWM(std::string, std::shared_ptr<ClimateActuatorPWM>);

private:
    std::shared_ptr<Communication::CommunicationController> controller;
    std::map<std::string, std::vector<std::shared_ptr<ClimateSensor>>> sensors;
    std::map<std::string, std::vector<std::shared_ptr<ClimateActuatorPWM>>> pwm_actuators;

    std::map<uint16_t, std::shared_ptr<ClimateSensor>> sensor_address_map;

};

#endif