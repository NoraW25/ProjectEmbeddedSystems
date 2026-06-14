
/*!
 * @file MessageTranslator.h
 * @class MessageTranslator
 * @brief Vertaler voor TCP sockets.
 * 
 * De universele vertaler voor datastromen door TCP sockets.
 * Deze vertaler vertaalt berichten van en naar de standaard vorm die de
 * Communication package verwacht. Dus de int als adres en de 
 * vector<uint8_t> waar data in meegegeven kan worden.
 */

#ifndef MESSAGETRANSLATOR_H
#define MESSAGETRANSLATOR_H

#include <string>
#include <vector>
#include <iostream>
#include <cstdint>

class MessageTranslator{
public:
    virtual ~MessageTranslator();

    /*!
     * @brief Haalt de instantie van de MessageTranslator op.
     *
     * Deze statische methode zorgt ervoor dat er maar 1 instantie bestaat 
     * van de MessageTranslator in een proces.
     * Eerst wordt er bekeken of er al een instantie bestaat van MessageTranslator.
     * - Als deze bestaat wordt meteen de instantie teruggegeven.
     * - Als deze niet bestaat wordt er een MessageTranslator object aangemaakt. 
     *   Daarvan wordt vervolgens de instantie teruggegeven.
     * 
     * @return MessageTranslator* - Een pointer naar het enige object van MessageTranslator in dat proces.
     */
    static MessageTranslator* instance();

    /*!
     * @brief Vertaalt een TCP bericht naar een bruikbaar bericht.
     *
     * Gebruik deze methode als er een bericht is ontvangen op een TCP socket.
     * Deze methode haalt eerst het adres uit het bericht, vervolgens
     * wordt ook de data uit het bericht gehaald.
     * 
     * @param int* - Geef een pointer naar een intergergeheugenplek mee waar het opgehaalde adres naartoe geschreven kan worden.
     * @param vector<uint8_t>* - Geef een pointer naar een vector mee waar de opgehaalde data naartoe geschreven kan worden.
     */
    virtual void translate(int*, std::vector<uint8_t>*, std::string);

    /*!
     * @brief Vertaalt een bericht naar een TCP bericht.
     *
     * Gebruik deze methode als er een bericht moet worden verzonden op een TCP socket.
     * Deze methode maakt een std::string van de meegegeven elementen die meegegeven kan 
     * worden bij het verzenden in een TCP socket.
     * 
     * @param int - Geef het adres/ID mee.
     * @param vector<uint8_t> - Geef data mee.
     * 
     * @return std::string - Een string die aan de TCP socket meegegeven kan worden.
     */
    virtual std::string translate(int, std::vector<uint8_t>);


private:
    MessageTranslator();
    static MessageTranslator* pointer_instance;

    /*!
     * @brief Vertaalt het adres/ID uit een TCP bericht naar bruikbare vorm.
     * 
     * @param string& - Een constante std::string referentie van het verzonden bericht.
     * @param int* - Een pointer naar de geheugenplek waar het adres/ID ingeplaatst kan worden.
     * 
     * @return bool - Geeft terug of het vertalen gelukt is. True bij gelukt, False bij mislukt.
     */
    bool parseId(const std::string&, int*);

    /*!
     * @brief Vertaalt de data uit een TCP bericht naar een bruikbare vorm.
     * 
     * @param string& - Een constante std::string referentie van het verzonden bericht.
     * @param vector<uint8_t>* - Een pointer naar de geheugenplek waar de data ingeplaatst kan worden.
     * 
     * @return bool - Geeft terug of het vertalen gelukt is. True bij gelukt, False bij mislukt.
     */
    bool parseData(const std::string&, std::vector<uint8_t>*);

    /*!
     * @brief Vertaalt een adres/ID naar een string voor een TCP bericht.
     * 
     * @param int - Geef het adres/ID mee.
     * 
     * @return std::string - Het adres/ID in de vorm van een std::string.
     */
    std::string stringifyId(int);

    /*!
     * @brief Vertaalt de data naar een string voor een TCP bericht.
     * 
     * @param vector<uint8_t>& - Geef een referentie naar de data mee.
     * 
     * @return std::string - De data in de vorm van een std::string.
     */
    std::string stringifyData(std::vector<uint8_t>&);

    std::string key_id;
    std::string key_data;
};

#endif