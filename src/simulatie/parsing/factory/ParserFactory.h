
#ifndef PARSER_FACTORY_H
#define PARSER_FACTORY_H

#include "../interfaces/IParserStrategy.h"
#include <memory>
#include <map>
#include <vector>

/**
 * @brief Factory class voor het beheren van parser strategies
 * 
 * Deze klasse implementeert het Factory pattern voor het automatisch
 * selecteren van de juiste parser strategy op basis van bestandsextensie.
 */
class ParserFactory {
private:
    static std::map<std::string, std::unique_ptr<IParserStrategy>> strategies;
    static bool initialized;

public:
    /**
     * @brief Initialiseert de factory met alle standaard strategies
     * @post initialized == true
     * @post Alle standaard strategies zijn geregistreerd
     */
    static void initialize();

    /**
     * @brief Registreert een nieuwe parser strategy voor een extensie
     * @param extension De bestandsextensie (bijv. ".xml", ".json")
     * @param strategy Unique pointer naar de strategy implementatie
     * @pre strategy != nullptr
     * @post Strategy is geregistreerd voor de gegeven extensie
     */
    static void registerStrategy(const std::string& extension, std::unique_ptr<IParserStrategy> strategy);

    /**
     * @brief Krijgt de juiste parser voor een bestand
     * @param filename Het bestand om te parsen
     * @return Parser strategy of nullptr als extensie niet ondersteund wordt
     * @post Als result != nullptr: result ondersteunt de extensie van filename
     */
    static IParserStrategy* getParserForFile(const std::string& filename);

    /**
     * @brief Geeft alle ondersteunde extensies terug
     * @return vector van ondersteunde extensies
     * @post Retourneert alle geregistreerde extensies
     */
    static std::vector<std::string> getSupportedExtensions();

    /**
     * @brief Geeft namen van alle geregistreerde strategies terug
     * @return vector van strategy namen
     */
    static std::vector<std::string> getRegisteredStrategyNames();

    /**
     * @brief Controleert of een extensie ondersteund wordt
     * @param extension De te controleren extensie
     * @return true als extensie ondersteund wordt
     */
    static bool isExtensionSupported(const std::string& extension);

    /**
     * @brief Reset de factory (vooral voor testing)
     * @post initialized == false && strategies is leeg
     */
    static void reset();
};

#endif // PARSER_FACTORY_H