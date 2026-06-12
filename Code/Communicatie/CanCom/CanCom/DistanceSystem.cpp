
#include "DistanceSystem.h"


DistanceSystem::DistanceSystem(std::shared_ptr<Communication::CommunicationController> can_controller, 
    std::shared_ptr<Communication::CommunicationController> rpib_controller):
        distance(0),
        amount_of_lights(0),
        finish_distance(20),
        amount_of_lights_available(9),
        can_controller(can_controller),
        rpib_controller(rpib_controller),

        address_turns(512),
        address_reset_button(511),
        address_display(720)
{
    can_controller->logReceived(address_turns,
                            [this](std::vector<uint8_t> data)
                            { addToDistance(data); });
    can_controller->logReceived(address_reset_button,
                            [this](std::vector<uint8_t> data)
                            { resetDistance(data); });
    rpib_controller->logReceived(address_turns,
                            [this](std::vector<uint8_t> data)
                            { addToDistance(data); });

}

DistanceSystem::DistanceSystem(std::shared_ptr<Communication::CommunicationController> can_controller, 
    std::shared_ptr<Communication::CommunicationController> rpib_controller, 
    int address_turns,
    int address_reset_button,
    int address_display):
        distance(0),
        amount_of_lights(0),
        finish_distance(20),
        amount_of_lights_available(9),
        can_controller(can_controller),
        rpib_controller(rpib_controller),

        address_turns(address_turns),
        address_reset_button(address_reset_button),
        address_display(address_display)
{
    can_controller->logReceived(address_turns,
                            [this](std::vector<uint8_t> data)
                            { addToDistance(data); });
    can_controller->logReceived(address_reset_button,
                            [this](std::vector<uint8_t> data)
                            { resetDistance(data); });

}

void DistanceSystem::resetDistance(std::vector<uint8_t> data){
    printf("Debug in resetDistance\n");
    distance = 0;
    amount_of_lights = 0;
    calculateAmountOfLights();
}

void DistanceSystem::addToDistance(std::vector<uint8_t> data){
    distance++;
    calculateAmountOfLights();
}

void DistanceSystem::calculateAmountOfLights(){

    amount_of_lights = (double) (((double) distance / (double) finish_distance)) * amount_of_lights_available;

    std::vector<uint8_t> data;
    int amount_of_bytes = sizeof(int);
    
    // LSB eerst
    for (int i = 0; i < amount_of_bytes; i++){
        uint8_t byte_value = (amount_of_lights >> (8*i)) & 0xFF;
        data.push_back(byte_value);
    }

    printf("amount of lights: %d\n", amount_of_lights);

    rpib_controller->transmitData(address_display, data);
}