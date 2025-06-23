#ifndef XML_PARSER_STRATEGY_H
#define XML_PARSER_STRATEGY_H

#include "../interfaces/IParserStrategy.h"
#include "../../../TinyXML/tinyxml.h"
#include "../../../logs_and_errors/ErrorOutput.h"
#include "../../../elementen/Baan.h"
#include <map>

class Simulator;

/**
 * @brief Concrete implementation voor XML file parsing
 * 
 * Deze klasse implementeert de IParserStrategy interface voor XML bestanden.
 * Het gebruikt TinyXML voor de daadwerkelijke XML parsing.
 */
class XMLParserStrategy : public IParserStrategy {
private:
    static std::map<std::string, Baan*> banenMap;

    /**
     * @brief Helper methods voor het parsen van specifieke XML elementen
     * Deze methods zijn direct gekopieerd en aangepast van je originele Parser.cpp
     */
    void parseBanen(TiXmlElement* elem, Simulator* sim, ErrorOutput& errorOutput);
    void parseVoertuigen(TiXmlElement* elem, ErrorOutput& errorOutput);
    void parseVerkeerslichten(TiXmlElement* elem, ErrorOutput& errorOutput);
    void parseBushaltes(TiXmlElement* elem, ErrorOutput& errorOutput);
    void parseKruisPunten(TiXmlElement* elem, ErrorOutput& errorOutput);
    void parseVoertuiggeneratoren(TiXmlElement* elem, ErrorOutput& errorOutput);

    /**
     * @brief Helper method voor error handling
     */
    void exceptionFound(bool& geldig, const std::string& message, ErrorOutput& errorOutput);

    /**
     * @brief Controleert of een voertuigtype geldig is
     */
    bool geldigeTypen(const std::string& type);

public:
    /**
     * @brief Parse een XML bestand
     * @param filename Pad naar XML bestand
     * @param sim Simulator om objecten aan toe te voegen
     * @param errorOutput Error handling
     * @return true bij success
     */
    bool parseFile(const std::string& filename, Simulator* sim, ErrorOutput& errorOutput) override;

    /**
     * @brief Geeft ondersteunde extensies terug
     * @return {".xml"}
     */
    std::vector<std::string> getSupportedExtensions() const override;

    /**
     * @brief Geeft strategy naam terug
     * @return "XML Parser"
     */
    std::string getStrategyName() const override;


};

#endif // XML_PARSER_STRATEGY_H