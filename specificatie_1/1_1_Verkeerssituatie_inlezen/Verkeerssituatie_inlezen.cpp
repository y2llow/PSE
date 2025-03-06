#include <iostream>
#include <string>
#include <vector>

#include "../../src/TinyXML/tinyxml.h"
#include "../Elementen/BAAN.h"
#include "../Elementen/VOERTUIG.h"
#include "../Elementen/VERKEERSLICHT.h"
#include "../Elementen/VOERTUIGGENERATOR.h"

std::vector<BAAN> banen;
std::vector<VOERTUIG> voertuigen;
std::vector<VERKEERSLICHT> verkeerslichten;
std::vector<VOERTUIGGENERATOR> voertuiggeneratoren;


void printXMLElements(const std::string& filename) {
    TiXmlDocument doc;
    if (!doc.LoadFile(filename.c_str())) {
        std::cerr << "Error loading file: " << doc.ErrorDesc() << std::endl;
        return;
    }

    TiXmlElement* root = doc.FirstChildElement();
    if (!root) {
        std::cerr << "Failed to load file: No root element." << std::endl;
        return;
    }

    // Loop door alle elementen
    for (TiXmlElement* elem = root; elem != nullptr; elem = elem->NextSiblingElement()) {
        std::cout << "Element: " << elem->Value() << std::endl;


        // Loop door de subelementen van elk element
        for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr; subElem = subElem->NextSiblingElement()) {


            std::cout << "  " << subElem->Value() << ": " << (subElem->GetText() ? subElem->GetText() : "N/A") << std::endl;
        }
    }
}


