
#ifndef IPARSER_STRATEGY_H
#define IPARSER_STRATEGY_H

#include <string>
#include <vector>
#include <algorithm>  // 🔧 FIX: Voor std::find

// Forward declarations
class Simulator;
class ErrorOutput;

/**
 * @brief Abstract base class voor alle parser strategieën
 *
 * Deze interface definieert het contract dat alle concrete parsers moeten implementeren.
 * Elke parser strategy is verantwoordelijk voor het parsen van een specifiek bestandsformaat.
 */
class IParserStrategy {
public:
    /**
     * @brief Virtual destructor voor proper cleanup
     */
    virtual ~IParserStrategy() = default;

    /**
     * @brief Parse een bestand naar simulator objecten
     * @param filename Het pad naar het bestand dat geparst moet worden
     * @param sim De simulator waaraan objecten toegevoegd moeten worden
     * @param errorOutput Voor error logging en reporting
     * @return true bij succesvolle parsing, false bij failure
     * @pre sim != nullptr
     * @post Bij success: objecten zijn toegevoegd aan sim
     */
    virtual bool parseFile(const std::string& filename, Simulator* sim, ErrorOutput& errorOutput) = 0;

    /**
     * @brief Geeft de ondersteunde file extensies terug
     * @return vector van extensies (bijv. {".xml", ".json"})
     * @post Retourneert non-empty vector met geldige extensies
     */
    virtual std::vector<std::string> getSupportedExtensions() const = 0;

    /**
     * @brief Geeft de naam van de parser strategie terug
     * @return Leesbare naam van deze parser (bijv. "XML Parser")
     * @post Retourneert non-empty string
     */
    virtual std::string getStrategyName() const = 0;

    /**
     * @brief Controleert of deze parser een specifieke extensie ondersteunt
     * @param extension De te controleren extensie (inclusief punt, bijv. ".xml")
     * @return true als deze parser de extensie kan verwerken
     */
    virtual bool supportsExtension(const std::string& extension) const {
        auto extensions = getSupportedExtensions();
        return std::find(extensions.begin(), extensions.end(), extension) != extensions.end();
    }
};

#endif // IPARSER_STRATEGY_H