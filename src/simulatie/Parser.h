//
// Created by Abdellah on 4/28/2025.
//

#ifndef PARSER_H
#define PARSER_H
#include <map>
#include <string>

#include "Simulator.h"
#include "../TinyXML/tinyxml.h"

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
    }

    /**
     * @brief Verwerkt een XML-bestand en maakt objecten aan in de simulator
     * @pre properlyInit() == true && filename is niet leeg && sim != nullptr
     * @return true als het bestand succesvol is verwerkt
     */
    static bool parseElements(const std::string& filename, Simulator* sim);

    /**
     * @brief Verwerkt banen uit de XML
     * @pre properlyInit() == true && elem != nullptr && sim != nullptr
     */
    static void parseBanen(TiXmlElement* elem, Simulator* sim);

    /**
     * @brief Verwerkt voertuigen uit de XML
     * @pre properlyInit() == true && elem != nullptr && sim != nullptr
     */
    static void parseVoertuigen(TiXmlElement* elem);

    /**
     * @brief Verwerkt kruispunten uit de XML
     * @pre properlyInit() == true && elem != nullptr && sim != nullptr
     */
    static void parseKruisPunten(TiXmlElement* elem);

    /**
     * @brief Verwerkt verkeerslichten uit de XML
     * @pre properlyInit() == true && elem != nullptr && sim != nullptr
     */
    static void parseVerkeerslichten(TiXmlElement* elem);

    /**
     * @brief Verwerkt bushaltes uit de XML
     * @pre properlyInit() == true && elem != nullptr && sim != nullptr
     */
    static void parseBushaltes(TiXmlElement* elem);

    /**
     * @brief Verwerkt voertuiggeneratoren uit de XML
     * @pre properlyInit() == true && elem != nullptr && sim != nullptr
     */
    static void parseVoertuiggeneratoren(TiXmlElement* elem);

    /**
     * @brief Markeert een exception in het verwerkingsproces
     * @pre properlyInit() == true
     */
    static void exceptionFound(bool& geldig, const string& message);

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