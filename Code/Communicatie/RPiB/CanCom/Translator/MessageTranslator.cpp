
#include "MessageTranslator.h"

MessageTranslator *MessageTranslator::pointer_instance = 0;

MessageTranslator::MessageTranslator() : key_id("ID:"),
                                         key_data("DATA:")
{
}

MessageTranslator *MessageTranslator::instance()
{
    if (pointer_instance == 0)
    {
        pointer_instance = new MessageTranslator();
    }

    return pointer_instance;
}

MessageTranslator::~MessageTranslator()
{
    pointer_instance = 0;
}

void MessageTranslator::translate(int *id, std::vector<uint8_t> *data, std::string message)
{
    std::cout << "Het ontvangen bericht: " << message << std::endl;
    if (!parseId(message, id))
    {
        std::cout << "Error in de parseId" << std::endl;
    }

    if (!parseData(message, data))
    {
        std::cout << "Error in de parseData" << std::endl;
    }
}

std::string MessageTranslator::translate(int id, std::vector<uint8_t> data)
{
    std::string text = key_id + stringifyId(id) + ";" + key_data + stringifyData(data);
    return text;
}

bool MessageTranslator::parseId(const std::string &message, int *id)
{
    size_t key = message.find(key_id) + key_id.length();
    size_t value = message.find(";", key);

    try
    {
        *id = std::stoi(message.substr(key, value - key));
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool MessageTranslator::parseData(const std::string &message, std::vector<uint8_t> *data)
{
    std::vector<uint8_t> result;

    size_t position = message.find(key_data);
    if (position == std::string::npos)
    {
        return false;
    }

    position += key_data.length();

    while (result.size() < 8)
    {
        size_t next = message.find(";", position);
        if (next == std::string::npos)
        {
            if (position >= message.size())
            {
                break;
            }

            std::string token = message.substr(position);
            if (!token.empty())
            {
                try
                {
                    int value = std::stoi(token);
                    result.push_back((uint8_t)value);
                }
                catch (...)
                {
                    return false;
                }
            }
            break;
        }

        if (next > position)
        {
            std::string token = message.substr(position, next - position);
            try
            {
                int value = std::stoi(token);
                result.push_back((uint8_t)value);
            }
            catch (...)
            {
                return false;
            }
        }

        position = next + 1;
    }

    if (result.size() >= 8)
    {
        return false;
    }

    *data = result;
    return true;
}

std::string MessageTranslator::stringifyId(int id)
{
    return std::to_string(id);
}

std::string MessageTranslator::stringifyData(std::vector<uint8_t> &data)
{
    std::string text;

    for (int it = 0; it < data.size(); it++)
    {
        text += std::to_string(static_cast<int>(data[it])) + ";";
    }

    return text;
}