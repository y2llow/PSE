//
// Created by Abdellah on 5/11/2025.
//

#ifndef XMLPARSER_H
#define XMLPARSER_H
#include <map>
#include <string>
#include <vector>
#include "../TinyXML/tinyxml.h"


class Verkeerslicht;
class Voertuiggenerator;
class Kruispunt;
class Bushalte;
class Voertuig;
class TiXmlElement;
class Baan;
class Simulator;

using namespace std;

class XMLParser
{
    map<string, Baan*> banen_map;

public:
    static Kruispunt* parseKruispunt(TiXmlElement* elem);
    static Bushalte* parseBushalte(TiXmlElement* elem);
    static Voertuig* parseVoertuig(TiXmlElement* elem);
    static Baan* parseBaan(TiXmlElement* elem);
    static Voertuiggenerator* parseVoertuiggenerator(TiXmlElement* elem);
    static Verkeerslicht* parseVerkeerslicht(TiXmlElement* elem);

    vector<Baan*> parseElements(const string& filename);
    static Simulator* makeSimulator();

    static void connectElements(const vector<Baan*>& bannen, const vector<Voertuig*>& voertuigen,
                                const vector<Verkeerslicht*>& verkeerslichts, const vector<Bushalte*>& bushaltes,
                                const vector<Kruispunt*>& kruispunts);
};


#endif //XMLPARSER_H
