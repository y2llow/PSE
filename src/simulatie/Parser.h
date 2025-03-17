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
    bool parseElements(const std::string &filename, simulation* sim);

    void parseBanen(TiXmlElement* root, simulation* sim);
    void parseOtherElements(TiXmlElement* root, simulation* sim);
};



#endif //PARSER_H
