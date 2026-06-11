
#include "LightingSystem.h"

#define COLOURORANGE "oranje"
#define COLOURYELLOW "geel"
#define COLOURBLUE "blauw"
#define COLOURRED "rood"
#define COLOURGREEN "groen"
#define COLOURPURPLE "paars"
#define COLOURCYAAN "cyaan"
#define COLOURWHITE "wit"


LightingSystem::LightingSystem(int button_change_colour_address, int button_off_address, std::shared_ptr<Communication::CommunicationController> controller):
    controller(controller),
    button_address(button_address),
    lights_off_address(509),
    lights_on(true){

    // Koppel controller aan verander kleur, wordt geactiveerd wanneer de knop wordt ingedrukt.
    controller->logReceived(button_change_colour_address, 
        [this](std::vector<uint8_t> data) { changeColour(data); } 
        // Om this af te vangen en alleen het type void (*)(std::vector<uint8_t>) mee te geven ipv void (ClimateSensor::*)(std::vector<uint8_t>)
    ); 

    controller->logReceived(button_off_address, 
        [this](std::vector<uint8_t> data) { toggleLight(data); } 
        // Om this af te vangen en alleen het type void (*)(std::vector<uint8_t>) mee te geven ipv void (ClimateSensor::*)(std::vector<uint8_t>)
    );

    colours[COLOURORANGE] = 501;
    colours[COLOURRED] = 502;
    colours[COLOURGREEN] = 503;
    colours[COLOURBLUE] = 504;
    colours[COLOURPURPLE] = 505;
    colours[COLOURYELLOW] = 506;
    colours[COLOURCYAAN] = 507;
    colours[COLOURWHITE] = 508;

    current_colour = colours.begin();
}

void LightingSystem::changeColour(std::vector<uint8_t> data){

    if (lights_on == true){
        current_colour++;

        if (current_colour == colours.end()){
            current_colour = colours.begin();
        }

        sendNewColour();
    }
    
}

void LightingSystem::toggleLight(std::vector<uint8_t> d){
    std::vector<uint8_t> data;
    data.push_back(0);

    if (lights_on){
        lights_on = false;
        sendNewColour();
    } else {
        lights_on = true;
        controller->transmitData(lights_off_address, data);
    }    
}

void LightingSystem::sendNewColour(){
    std::vector<uint8_t> data;
    data.push_back(0);

    int address = current_colour->second;
    controller->transmitData(address, data);
}