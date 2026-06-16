
#include "ClimateSystem.h"

#define TEMPERATURESENSORS "temperature_sensors"
#define CO2SENSORS "co2_sensors"
#define HUMIDITYSENSORS "humidity_sensors"
#define VENTILATORS "ventilators"

#include <iostream>

ClimateSystem::ClimateSystem(std::shared_ptr<Communication::CommunicationController> controller,
                             std::shared_ptr<Communication::CommunicationController> controller_rpia,
                             int buzzer_address) : controller(controller),
                                                   controller_rpia(controller_rpia),
                                                   buzzer_address(buzzer_address)
{

    // Sensoren aanmaken
    addSensor(TEMPERATURESENSORS, std::make_shared<ClimateSensor>(610, controller, this));
    addSensor(CO2SENSORS, std::make_shared<ClimateSensor>(620, controller, this));
    addSensor(HUMIDITYSENSORS, std::make_shared<ClimateSensor>(630, controller, controller_rpia, this));

    // Actuatoren aanmaken
    addActuatorPWM(VENTILATORS, std::make_shared<ClimateActuatorPWM>(640, controller));
}

void ClimateSystem::calculateSettings()
{
    std::map<std::string, std::vector<std::shared_ptr<ClimateSensor>>>::iterator it = sensors.begin();

    int avg_temperature = 0;
    int avg_co2 = 0;
    int avg_humidity = 0;
    int vensetting = 0;

    for (it; it != sensors.end(); it++)
    {
        const std::string &type = it->first;
        const std::vector<std::shared_ptr<ClimateSensor>> sensors_of_type = it->second;

        int sum_type = 0;

        auto sensor = sensors_of_type.begin();
        for (; sensor != sensors_of_type.end(); sensor++)
        {
            sum_type += (*sensor)->getCurrentValue();
        }

        std::cout << "type: " << type << " Waarde: " << sum_type << std::endl;

        if (type == TEMPERATURESENSORS)
        {
            sum_type /= sensors_of_type.size();
            if (sum_type > 25)
            {
                printf("temp+3\n");
                vensetting += 3;
            }
            else if (sum_type > 22)
            {
                printf("temp+2\n");
                vensetting += 2;
            }
            else if (sum_type > 20)
            {
                printf("temp+1\n");
                vensetting += 1;
            }
        }

        if (type == HUMIDITYSENSORS)
        {
            sum_type /= sensors_of_type.size();
            if (sum_type > 80)
            {
                printf("hum+2\n");
                vensetting += 2;
            }
            else if (sum_type > 70)
            {
                printf("hum+1\n");
                vensetting += 1;
            }

            std::vector<uint8_t> data;
            data.push_back(sum_type);

            // Verzendt data naar humidity sensor op de STM via RPiA
            controller_rpia->transmitData(150, data);
        }

        if (type == CO2SENSORS)
        {
            printf("IN CO2 SENSOR DATAVERWERKING\n");

            sum_type /= sensors_of_type.size();
            if (sum_type > 1200)
            {
                vensetting += 4;
                printf("CO2 1200 + 4\n");
                std::vector<uint8_t> data;
                int amount_of_bytes = sizeof(int);

                // LSB eerst
                for (int i = 0; i < amount_of_bytes; i++)
                {
                    uint8_t byte_value = (sum_type >> (8 * i)) & 0xFF;
                    data.push_back(byte_value);
                }

                // Zend een alarm door naar RPiA
                controller_rpia->transmitData(buzzer_address, data);
            }
            else if (sum_type > 800)
            {
                vensetting += 3;
                printf("CO2 800 + 3\n");
                std::vector<uint8_t> data;
                int amount_of_bytes = sizeof(int);

                // LSB eerst
                for (int i = 0; i < amount_of_bytes; i++)
                {
                    uint8_t byte_value = (sum_type >> (8 * i)) & 0xFF;
                    data.push_back(byte_value);
                }

                // Zend een alarm door naar RPiA
                controller_rpia->transmitData(buzzer_address, data);
            }
            else if (sum_type > 650)
            {
                vensetting += 2;
                printf("CO2 650 + 2\n");
            }
        }
    }

    std::map<std::string, std::vector<std::shared_ptr<ClimateActuatorPWM>>>::iterator it2 = pwm_actuators.begin();

    for (it2; it2 != pwm_actuators.end(); it2++)
    {
        const std::string &type = it2->first;
        const std::vector<std::shared_ptr<ClimateActuatorPWM>> actuator_of_type = it2->second;

        if (type == VENTILATORS)
        {
            int rest = vensetting % actuator_of_type.size();
            if (rest > 0)
            {
                vensetting += rest;
            }
            vensetting /= actuator_of_type.size();

            auto ventilator = actuator_of_type.begin();
            for (; ventilator != actuator_of_type.end(); ventilator++)
            {
                (*ventilator)->setPWM(vensetting * 10);
            }
        }
    }
}

void ClimateSystem::addActuatorPWM(std::string type, std::shared_ptr<ClimateActuatorPWM> actuator)
{
    std::map<std::string, std::vector<std::shared_ptr<ClimateActuatorPWM>>>::iterator it = pwm_actuators.begin();

    for (; it != pwm_actuators.end(); it++)
    {
        if (it->first == type)
        {
            (it->second).push_back(actuator);
            return;
        }
    }

    // Onbekende naam, dus nieuwe catogorie aanmaken
    pwm_actuators[type] = {actuator};
}

void ClimateSystem::addSensor(std::string type, std::shared_ptr<ClimateSensor> sensor)
{
    std::map<std::string, std::vector<std::shared_ptr<ClimateSensor>>>::iterator it = sensors.find(type);

    if (it != sensors.end())
    {
        it->second.push_back(sensor);
        return;
    }

    // Onbekende naam, dus nieuwe catogorie aanmaken
    sensors[type] = {sensor};
}