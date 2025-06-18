#ifndef PARSER_H
#define PARSER_H
#include <map>
#include <string>

#include "Simulator.h"
#include "../TinyXML/tinyxml.h"
#include "../logs_and_errors/ErrorOutput.h"

using namespace std;

class Parser
{
    static map<string, Baan*> banenMap;
    static Parser* _initCheck; // Voor properlyInit

public:
    /**
     * @brief Controleert of de klasse correct geïnitialiseerd is
     * @return true als deze klasse correct is geïnitialiseerd
     */
    static bool properlyInit() { return _initCheck != nullptr; }

    /**
     * @brief Initialiseert de Parser klasse
     * @post properlyInit() == true
     */
    static void initialize() {
        static Parser instance;
        _initCheck = &instance;
        banenMap.clear(); // Clear voor nieuwe parse sessie
    }

    /**
     * @brief Verwerkt een XML-bestand en maakt objecten aan in de simulator
     * @pre properlyInit() == true && filename is niet leeg && sim != nullptr
     * @return true als het bestand succesvol is verwerkt
     */
    static bool parseElements(const std::string& filename, Simulator* sim, ErrorOutput& errorOutput);

    /**
     * @brief Backwards compatibility - gebruik versie met ErrorOutput
     */
    static bool parseElements(const std::string& filename, Simulator* sim);

    /**
     * @brief Verwerkt banen uit de XML
     * @pre properlyInit() == true && elem != nullptr && sim != nullptr
     */
    static void parseBanen(TiXmlElement* elem, Simulator* sim, ErrorOutput& errorOutput);

    /**
     * @brief Verwerkt voertuigen uit de XML
     * @pre properlyInit() == true && elem != nullptr && sim != nullptr
     */
    static void parseVoertuigen(TiXmlElement* elem, ErrorOutput& errorOutput);

    /**
     * @brief Verwerkt kruispunten uit de XML
     * @pre properlyInit() == true && elem != nullptr && sim != nullptr
     */
    static void parseKruisPunten(TiXmlElement* elem, ErrorOutput& errorOutput);

    /**
     * @brief Verwerkt verkeerslichten uit de XML
     * @pre properlyInit() == true && elem != nullptr && sim != nullptr
     */
    static void parseVerkeerslichten(TiXmlElement* elem, ErrorOutput& errorOutput);

    /**
     * @brief Verwerkt bushaltes uit de XML
     * @pre properlyInit() == true && elem != nullptr && sim != nullptr
     */
    static void parseBushaltes(TiXmlElement* elem, ErrorOutput& errorOutput);

    /**
     * @brief Verwerkt voertuiggeneratoren uit de XML
     * @pre properlyInit() == true && elem != nullptr && sim != nullptr
     */
    static void parseVoertuiggeneratoren(TiXmlElement* elem, ErrorOutput& errorOutput);

    /**
     * @brief Markeert een exception in het verwerkingsproces
     * @pre properlyInit() == true
     */
    static void exceptionFound(bool& geldig, const string& message, ErrorOutput& errorOutput);

    /**
     * @brief Koppelt verkeerslichten op kruispunten aan elkaar
     * @pre properlyInit() == true
     * @post Verkeerslichten op kruispunten zijn aan elkaar gekoppeld
     */
    static void VerkeerslichtenOpKruispunten();

    /**
     * @brief Controleert of een voertuigtype geldig is
     * @pre properlyInit() == true
     * @return true als het type geldig is
     */
    static bool geldigeTypen(const string& type);
};

#endif //PARSER_H