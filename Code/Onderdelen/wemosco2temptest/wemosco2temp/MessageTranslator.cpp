
#include "MessageTranslator.h"

MessageTranslator *MessageTranslator::pointer_instance = 0;

MessageTranslator::MessageTranslator() : key_id("ID:"),
                                         key_data("DATA:")
{
}

/**
 * @brief Verkrijg singleton instantie van MessageTranslator
 * @return pointer naar de instantie
 */
MessageTranslator *MessageTranslator::instance()
{
    if (pointer_instance == 0)
    {
        pointer_instance = new MessageTranslator();
    }

    return pointer_instance;
}

/**
 * @brief Destructor zet singleton pointer terug naar null
 */
MessageTranslator::~MessageTranslator()
{
    pointer_instance = 0;
}

/**
 * @brief Parseer `message` en vul `id` en `data`
 */
void MessageTranslator::translate(int *id, std::vector<uint8_t> *data, std::string message)
{
    if (!parseId(message, id))
    {
        std::cout << "Error in de parseId" << std::endl;
    }

    if (!parseData(message, data))
    {
        std::cout << "Error in de parseData" << std::endl;
    }
}

/**
 * @brief Bouw een stringrepresentatie van id en data
 * @param id Bericht-id
 * @param data Data bytes
 * @return Geformatteerde string
 */
std::string MessageTranslator::translate(int id, std::vector<uint8_t> data)
{
    std::string text = key_id + stringifyId(id) + ";" + key_data + stringifyData(data);
    std::cout << "Size in translate: " << std::to_string(data.size()) << std::endl;
    std::cout << "Text in translate MessageTranslator: id + data naar string:\n"
              << text << "\n"
              << std::endl;
    return text;
}

/**
 * @brief Parse het id uit de message string
 * @param message Invoermessage
 * @param id Uitvoerparameter
 * @return true bij succesvolle parse
 */
bool MessageTranslator::parseId(const std::string &message, int *id)
{
    size_t key = message.find(key_id) + key_id.length();
    size_t value = message.find(";", key);

    *id = std::stoi(message.substr(key, value - key));

    return true;
}

/**
 * @brief Parse de data bytes uit de message string
 * @param message Invoermessage
 * @param data Uitvoerparameter voor de bytes
 * @return true bij succesvolle parse
 */
bool MessageTranslator::parseData(const std::string &message, std::vector<uint8_t> *data)
{
    std::vector<uint8_t> result;

    size_t position = message.find(key_data) + key_data.length();
    size_t next = message.find(";", position);

    while (next != std::string::npos)
    {
        printf("In while\n");

        int value = std::stoi(message.substr(position, next - position));
        result.push_back((uint8_t)value);

        position = next + 1;
        next = message.find(";", position);
    }

    *data = result;

    return true;
}

/**
 * @brief Converteer id naar string
 */
std::string MessageTranslator::stringifyId(int id)
{
    return std::to_string(id);
}

/**
 * @brief Converteer data bytes naar een puntgescheiden string
 */
std::string MessageTranslator::stringifyData(std::vector<uint8_t> &data)
{
    std::string text;

    std::cout << "Data size: " << std::to_string(data.size()) << std::endl;

    for (int it = 0; it < data.size(); it++)
    {
        text += std::to_string(static_cast<int>(data[it])) + ";";
    }

    return text;
}