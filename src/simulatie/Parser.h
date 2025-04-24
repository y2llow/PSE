//
// Created by s0246031@ad.ua.ac.be on 3/17/25.
//

#ifndef PARSER_H
#define PARSER_H
#include "simulation.h"

#include "../TinyXML/tinyxml.h"

using namespace std;
class Parser {
public:
    // Function to parse XML and create appropriate objects
    static bool parseElements(const std::string &filename, simulation* sim);

    static void parseBanen(TiXmlElement* root, simulation* sim);
    static void parseVoertuigen(TiXmlElement *root, simulation *sim);
    static void parseVerkeerslichten(TiXmlElement* root, simulation* sim);
    static void parseVoertuiggeneratoren(TiXmlElement *root, simulation *sim);
};
#endif //PARSER_H
