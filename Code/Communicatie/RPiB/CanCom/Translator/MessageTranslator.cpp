
#include "MessageTranslator.h"

MessageTranslator* MessageTranslator::pointer_instance = 0;

MessageTranslator::MessageTranslator():
    key_id("ID:"),
    key_data("DATA:"){

}

MessageTranslator* MessageTranslator::instance() {
	if (pointer_instance == 0) {
		pointer_instance = new MessageTranslator();
	}

	return pointer_instance;
}

MessageTranslator::~MessageTranslator(){
    pointer_instance = 0;
}

void MessageTranslator::translate(int* id, std::vector<uint8_t>* data, std::string message){
    std::cout<<"Het ontvangen bericht: " << message << std::endl;
    if (!parseId(message, id)){
        std::cout<<"Error in de parseId"<<std::endl;
    }

    if (!parseData(message, data)){
        std::cout<<"Error in de parseData"<<std::endl;
    }
}

std::string MessageTranslator::translate(int id, std::vector<uint8_t> data){
    std::string text = key_id + stringifyId(id) + ";"+ key_data + stringifyData(data);
    std::cout<<"Size in translate: "<<std::to_string(data.size())<<std::endl;
    std::cout<<"Text in translate MessageTranslator: id + data naar string:\n"<<text<<"\n"<<std::endl;
    return text;
}

bool MessageTranslator::parseId(const std::string& message, int* id){
    size_t key = message.find(key_id) + key_id.length();
    size_t value = message.find(";", key);

    try{
        *id = std::stoi(message.substr(key, value - key));
    }catch(...){
        return false;
    }
    
    return true;
}

bool MessageTranslator::parseData(const std::string& message, std::vector<uint8_t>* data){
    std::vector<uint8_t> result;

    size_t position = message.find(key_data) + key_data.length();
    size_t next = message.find(";", position);


    int i = 0;
    while(next != std::string::npos && i < 8){
        printf("In while\n");
        try{
            if (next-1 != position){
                int value = std::stoi(message.substr(position, next-position));
                result.push_back((uint8_t) value);                
            }
            position = next + 1;
            next = message.find(";", position);          
        }catch(...){
            return false;
        }

        i++;    
    }

    *data = result;

    return true;
}


std::string MessageTranslator::stringifyId(int id){
    return std::to_string(id);
}

std::string MessageTranslator::stringifyData(std::vector<uint8_t>& data){
    std::string text;

    std::cout<<"Data size: "<<std::to_string(data.size())<<std::endl;

    for(int it = 0; it < data.size(); it++){
        text += std::to_string(static_cast<int>(data[it])) + ";";
    }

    return text;
}