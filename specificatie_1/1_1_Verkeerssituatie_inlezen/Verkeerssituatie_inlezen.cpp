#include <iostream>
#include <string>
#include "../../src/TinyXML/tinyxml.h"

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

        if (elem->Value() == "BAAN") {
            BAAN baan;
        }

        // Loop door de subelementen van elk element
        for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr; subElem = subElem->NextSiblingElement()) {

            if (elem->Value() == "BAAN") {
                baan.naam = subElem->Value() ;
            }

            std::cout << "  " << subElem->Value() << ": " << (subElem->GetText() ? subElem->GetText() : "N/A") << std::endl;
        }
    }
}


