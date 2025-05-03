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

public:
    // Function to parse XML and create appropriate objects
    static bool parseElements(const std::string& filename, Simulator* sim);
    static void parseBanen(TiXmlElement* elem, Simulator* sim);
    static void parseVoertuigen(TiXmlElement* elem, Simulator* sim);
    static void parseKruisPunten(TiXmlElement* elem, Simulator* sim);
    static void parseVerkeerslichten(TiXmlElement* elem, Simulator* sim);
    static void parseBushaltes(TiXmlElement* elem, Simulator* sim);
    static void parseVoertuiggeneratoren(TiXmlElement* elem, Simulator* sim);

    static void exceptionFound(bool& geldig, const string& message);

    static void VerkeerslichtenOpKruispunten();
};


#endif //PARSER_H
