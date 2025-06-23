#ifndef UNIVERSAL_PARSER_H
#define UNIVERSAL_PARSER_H

#include "factory/ParserFactory.h"
#include <string>

// Forward declarations
class Simulator;
class ErrorOutput;

/**
 * @brief Universal Parser die automatisch het juiste format detecteert
 * 
 * Deze klasse biedt backwards compatibility met de oude Parser API
 * terwijl het intern gebruik maakt van het nieuwe strategy pattern.
 */
class UniversalParser {
private:
    static UniversalParser* _initCheck;

public:
    /**
     * @brief Controleert of de klasse correct geïnitialiseerd is
     * @return true als deze klasse correct is geïnitialiseerd
     */
    static bool properlyInit() { return _initCheck != nullptr; }

    /**
     * @brief Initialiseert de Universal Parser
     * @post properlyInit() == true
     * @post ParserFactory is geïnitialiseerd
     */
    static void initialize();

    /**
     * @brief Parse elk ondersteund bestandsformaat automatisch
     * @param filename Het bestand om te parsen
     * @param sim De simulator
     * @param errorOutput Error handling
     * @return true bij success
     * @pre sim != nullptr
     * @post Bij success: bestand is geparst en objecten zijn toegevoegd aan sim
     */
    static bool parseFile(const std::string& filename, Simulator* sim, ErrorOutput& errorOutput);

    /**
     * @brief Backwards compatibility met oude Parser API
     * @param filename Het bestand om te parsen  
     * @param sim De simulator
     * @param errorOutput Error handling
     * @return true bij success
     */
    static bool parseElements(const std::string& filename, Simulator* sim, ErrorOutput& errorOutput);

    /**
     * @brief Backwards compatibility zonder ErrorOutput
     * @param filename Het bestand om te parsen
     * @param sim De simulator
     * @return true bij success
     */
    static bool parseElements(const std::string& filename, Simulator* sim);


};

#endif // UNIVERSAL_PARSER_H