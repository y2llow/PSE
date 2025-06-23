
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


};

#endif // PARSER_FACTORY_H