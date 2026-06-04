
#ifndef MESSAGETRANSLATOR_H
#define MESSAGETRANSLATOR_H

#include <string>
#include <vector>
#include <iostream>
#include <cstdint>

class MessageTranslator{
public:
    virtual ~MessageTranslator();

    static MessageTranslator* instance();

    virtual void translate(int*, std::vector<uint8_t>*, std::string);
    virtual std::string translate(int, std::vector<uint8_t>);


private:
    MessageTranslator();
    static MessageTranslator* pointer_instance;

    bool parseId(const std::string&, int*);
    bool parseData(const std::string&, std::vector<uint8_t>*);

    std::string stringifyId(int);
    std::string stringifyData(std::vector<uint8_t>&);

    std::string key_id;
    std::string key_data;
};

#endif