#ifndef JSON_PARSER_STRATEGY_H
#define JSON_PARSER_STRATEGY_H

#include "../interfaces/IParserStrategy.h"
#include "../../../logs_and_errors/ErrorOutput.h"
#include "../../../elementen/Baan.h"

// Check if JSON support is available
#ifdef JSON_SUPPORT_ENABLED
#include "../../../external/nlohmann/json.hpp"
#endif

#include <map>

class Simulator;

/**
 * @brief Concrete implementation voor JSON file parsing
 * 
 * Deze klasse implementeert de IParserStrategy interface voor JSON bestanden.
 * Het gebruikt nlohmann/json voor de daadwerkelijke JSON parsing.
 */
class JSONParserStrategy : public IParserStrategy {
private:
    static std::map<std::string, Baan*> banenMap;

#ifdef JSON_SUPPORT_ENABLED
    /**
     * @brief Helper methods voor het parsen van specifieke JSON elementen
     */
    void parseBanen(const nlohmann::json& banenArray, Simulator* sim, ErrorOutput& errorOutput);
    void parseVoertuigen(const nlohmann::json& voertuigenArray, ErrorOutput& errorOutput);
    void parseVerkeerslichten(const nlohmann::json& lichtenArray, ErrorOutput& errorOutput);
    void parseBushaltes(const nlohmann::json& haltesArray, ErrorOutput& errorOutput);
    void parseKruisPunten(const nlohmann::json& kruispuntenArray, ErrorOutput& errorOutput);
    void parseVoertuiggeneratoren(const nlohmann::json& generatorsArray, ErrorOutput& errorOutput);
    
    /**
     * @brief Helper method voor error handling
     */
    void exceptionFound(bool& geldig, const std::string& message, ErrorOutput& errorOutput);
    
    /**
     * @brief Controleert of een voertuigtype geldig is
     */
    bool geldigeTypen(const std::string& type);
    
    /**
     * @brief Safely get string from JSON with default value
     */
    std::string safeGetString(const nlohmann::json& obj, const std::string& key, const std::string& defaultValue = "");
    
    /**
     * @brief Safely get integer from JSON with default value
     */
    int safeGetInt(const nlohmann::json& obj, const std::string& key, int defaultValue = 0);
    
    /**
     * @brief Safely get double from JSON with default value
     */
    double safeGetDouble(const nlohmann::json& obj, const std::string& key, double defaultValue = 0.0);
#endif

public:
    /**
     * @brief Parse een JSON bestand
     * @param filename Pad naar JSON bestand
     * @param sim Simulator om objecten aan toe te voegen
     * @param errorOutput Error handling
     * @return true bij success
     */
    bool parseFile(const std::string& filename, Simulator* sim, ErrorOutput& errorOutput) override;

    /**
     * @brief Geeft ondersteunde extensies terug
     * @return {".json"} als JSON support enabled is, anders lege vector
     */
    std::vector<std::string> getSupportedExtensions() const override;

    /**
     * @brief Geeft strategy naam terug
     * @return "JSON Parser" of "JSON Parser (Disabled)"
     */
    std::string getStrategyName() const override;
};

#endif // JSON_PARSER_STRATEGY_H