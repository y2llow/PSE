//
// Created by Abdellah on 5/11/2025.
//

#include "XMLParser.h"

#include "../elementen/Voertuiggenerator.h"
#include "../elementen/Verkeerslicht.h"
#include "../elementen/Bushalte.h"
#include "../elementen/Baan.h"
#include "../elementen/Kruispunt.h"

#include <algorithm>
#include <iostream>

#include "../elementen/Voertuig.h"


Kruispunt* XMLParser::parseKruispunt(TiXmlElement* elem)
{
}

Bushalte* XMLParser::parseBushalte(TiXmlElement* elem)
{

}

Voertuig* XMLParser::parseVoertuig(TiXmlElement* elem)
{
}

Baan* XMLParser::parseBaan(TiXmlElement* elem)
{
}

Voertuiggenerator* XMLParser::parseVoertuiggenerator(TiXmlElement* elem)
{
}

Verkeerslicht* XMLParser::parseVerkeerslicht(TiXmlElement* elem)
{
}

vector<Baan*> XMLParser::parseElements(const string& filename)
{
    TiXmlDocument doc;

    vector<Baan*> bannen;
    vector<Voertuig*> voertuigen;
    vector<Voertuiggenerator*> voertuiggeneratoren;
    vector<Bushalte*> bushaltes;
    vector<Kruispunt*> kruispunten;
    vector<Verkeerslicht*> verkeerslichten;



    if (!doc.LoadFile(filename.c_str()))
    {
        cerr << "Error loading file: " << doc.ErrorDesc() << endl;
        return {};
    }

    TiXmlElement* root = doc.FirstChildElement();
    if (!root)
    {
        cerr << "Failed to load file: No root element." << endl;
        return {};
    }

    // =========== First parse banen because they are essential for the implementation of other elements =============
    for (auto ti_xml_element = root->FirstChildElement(); ti_xml_element != nullptr; ti_xml_element =
         ti_xml_element->NextSiblingElement())
    {
        if (string elementType = ti_xml_element->Value(); elementType != "BAAN")
            continue;
        bannen.push_back(parseBaan(ti_xml_element));
    }

    // =========== Just then parse the other elements =============
    for (auto ti_xml_element = root->FirstChildElement(); ti_xml_element != nullptr; ti_xml_element =
         ti_xml_element->NextSiblingElement())
    {
        if (string elementType = ti_xml_element->Value(); elementType == "VOERTUIG")
            voertuigen.push_back(parseVoertuig(ti_xml_element));
        else if (elementType == "VOERTUIGGENERATOR")
            voertuiggeneratoren.push_back(parseVoertuiggenerator(ti_xml_element));
        else if (elementType == "VERKEERSLICHT")
            verkeerslichten.push_back(parseVerkeerslicht(ti_xml_element));
        else if (elementType == "BUSHALTE")
            bushaltes.push_back(parseBushalte(ti_xml_element));
        else if (elementType == "KRUISPUNT")
            kruispunten.push_back(parseKruispunt(ti_xml_element));
    }

    connectElements(bannen, voertuigen, verkeerslichten, bushaltes, kruispunten);
    return bannen;
}

void XMLParser::connectElements(const vector<Baan*>& bannen, const vector<Voertuig*>& voertuigen,
    const vector<Verkeerslicht*>& verkeerslichts, const vector<Bushalte*>& bushaltes,
    const vector<Kruispunt*>& kruispunts)
{
    for (const auto v : voertuigen)
        v->getBaan()->addVoertuig(v);

    for (const auto v : verkeerslichts)
        v->getBaan()->addVerkeerslicht(v);

    for (const auto b : bushaltes)
        b->getBaan()->addBushalte(b);
}

