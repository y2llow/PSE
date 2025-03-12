#include <iostream>
#include <string>
#include <vector>
#include "Verkeerssituatie_inlezen.h"
#include "../../src/TinyXML/tinyxml.h"
#include "../../src/Elementen/Baan.h"
#include "../../src/Elementen/Voertuig.h"
#include "../../src/Elementen/Verkeerslicht.h"
#include "../../src/Elementen/Voertuiggenerator.h"

// Vectors to store objects created from XML
std::vector<Baan> banen;
std::vector<Voertuig> voertuigen;
std::vector<Verkeerslicht> verkeerslichten;
std::vector<Voertuiggenerator> voertuiggeneratoren;

// Function to parse XML and create appropriate objects
void parseXMLAndCreateObjects(const std::string& filename) {
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

    for (TiXmlElement* elem = root->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement()) {
        std::string elementType = elem->Value();

        if (elementType == "BAAN") {
            Baan baan;

            // Loop through sub-elements to get properties
            for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr; subElem = subElem->NextSiblingElement()) {
                std::string propertyName = subElem->Value();

                if (propertyName == "naam" && subElem->GetText()) {
                    baan.naam = subElem->GetText();
                } else if (propertyName == "lengte" && subElem->GetText()) {
                    baan.lengte = std::stoi(subElem->GetText());
                }
            }

            // Add to vector
            banen.push_back(baan);
        }
        else if (elementType == "VOERTUIG") {
            Voertuig voertuig;

            for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr; subElem = subElem->NextSiblingElement()) {
                std::string propertyName = subElem->Value();

                if (propertyName == "baan" && subElem->GetText()) {
                    voertuig.baan = subElem->GetText();
                } else if (propertyName == "positie" && subElem->GetText()) {
                    voertuig.positie = std::stoi(subElem->GetText());
                }
            }

            voertuigen.push_back(voertuig);
        }
        else if (elementType == "VERKEERSLICHT") {
            Verkeerslicht verkeerslicht;

            for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr; subElem = subElem->NextSiblingElement()) {
                std::string propertyName = subElem->Value();

                if (propertyName == "baan" && subElem->GetText()) {
                    verkeerslicht.baan = subElem->GetText();
                } else if (propertyName == "positie" && subElem->GetText()) {
                    verkeerslicht.positie = std::stoi(subElem->GetText());
                } else if (propertyName == "cyclus" && subElem->GetText()) {
                    verkeerslicht.cyclus = std::stoi(subElem->GetText());
                }
            }

            verkeerslichten.push_back(verkeerslicht);
        }
        else if (elementType == "VOERTUIGGENERATOR") {
            Voertuiggenerator generator;

            for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr; subElem = subElem->NextSiblingElement()) {
                std::string propertyName = subElem->Value();

                if (propertyName == "baan" && subElem->GetText()) {
                    generator.baan = subElem->GetText();
                } else if (propertyName == "frequentie" && subElem->GetText()) {
                    generator.frequentie = std::stoi(subElem->GetText());
                }
            }

            voertuiggeneratoren.push_back(generator);
        }
    }
}



