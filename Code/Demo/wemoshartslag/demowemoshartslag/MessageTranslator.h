
#ifndef MESSAGETRANSLATOR_H
#define MESSAGETRANSLATOR_H

#include <string>
#include <vector>
#include <iostream>
#include <cstdint>

/**
 * @file MessageTranslator.h
 * @brief Hulpcodering om berichten met ID en DATA te parsen en samen te stellen
 */

/**
 * @class MessageTranslator
 * @brief Singleton die berichten vertaalt tussen string- en binaire representatie
 */
class MessageTranslator
{
public:
    /**
     * @brief Destructor
     */
    virtual ~MessageTranslator();

    /**
     * @brief Verkrijg singleton instantie
     * @return Pointer naar singleton
     */
    static MessageTranslator *instance();

    /**
     * @brief Parse een berichtstring naar id en data
     * @param id Uitvoerparameter voor het id
     * @param data Uitvoerparameter voor de data bytes
     * @param message Invoermessage als string
     */
    virtual void translate(int *, std::vector<uint8_t> *, std::string);

    /**
     * @brief Bouw een berichtstring vanuit id en data
     * @param id Bericht-id
     * @param data Data bytes
     * @return Geformatteerde string met id en data
     */
    virtual std::string translate(int, std::vector<uint8_t>);

private:
    MessageTranslator();
    static MessageTranslator *pointer_instance;

    bool parseId(const std::string &, int *);
    bool parseData(const std::string &, std::vector<uint8_t> *);

    std::string stringifyId(int);
    std::string stringifyData(std::vector<uint8_t> &);

    std::string key_id;
    std::string key_data;
};

#endif