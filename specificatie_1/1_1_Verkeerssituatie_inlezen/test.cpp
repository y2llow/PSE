#include <iostream>
#include <string>
#include <vector>
#include "../../src/TinyXML/tinyxml.h"
#include "../../src/Elementen/BAAN.h"
#include "../../src/Elementen/VOERTUIG.h"
#include "../../src/Elementen/VERKEERSLICHT.h"
#include "../../src/Elementen/VOERTUIGGENERATOR.h"

// Vectors to store objects created from XML
std::vector<BAAN> banen;
std::vector<VOERTUIG> voertuigen;
std::vector<VERKEERSLICHT> verkeerslichten;
std::vector<VOERTUIGGENERATOR> voertuiggeneratoren;

// Function to parse XML and create appropriate objects
void parseXMLAndCreateObjects1(const std::string& filename) {
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

    // Loop through all elements
    for (TiXmlElement* elem = root; elem != nullptr; elem = elem->NextSiblingElement()) {
        std::string elementType = elem->Value();

        if (elementType == "BAAN") {
            BAAN baan;

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
            std::cout << "Added BAAN: " << baan.naam << ", lengte: " << baan.lengte << std::endl;
        }
        else if (elementType == "VOERTUIG") {
            VOERTUIG voertuig;

            for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr; subElem = subElem->NextSiblingElement()) {
                std::string propertyName = subElem->Value();

                if (propertyName == "baan" && subElem->GetText()) {
                    voertuig.baan = subElem->GetText();
                } else if (propertyName == "positie" && subElem->GetText()) {
                    voertuig.positie = std::stoi(subElem->GetText());
                }
            }

            voertuigen.push_back(voertuig);
            std::cout << "Added VOERTUIG: on baan " << voertuig.baan << ", positie: " << voertuig.positie << std::endl;
        }
        else if (elementType == "VERKEERSLICHT") {
            VERKEERSLICHT verkeerslicht;

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
            std::cout << "Added VERKEERSLICHT: on baan " << verkeerslicht.baan
                      << ", positie: " << verkeerslicht.positie
                      << ", cyclus: " << verkeerslicht.cyclus << std::endl;
        }
        else if (elementType == "VOERTUIGGENERATOR") {
            VOERTUIGGENERATOR generator;

            for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr; subElem = subElem->NextSiblingElement()) {
                std::string propertyName = subElem->Value();

                if (propertyName == "baan" && subElem->GetText()) {
                    generator.baan = subElem->GetText();
                } else if (propertyName == "frequentie" && subElem->GetText()) {
                    generator.frequentie = std::stoi(subElem->GetText());
                }
            }

            voertuiggeneratoren.push_back(generator);
            std::cout << "Added VOERTUIGGENERATOR: on baan " << generator.baan
                      << ", frequentie: " << generator.frequentie << std::endl;
        }
    }
}

// Function to print all objects (for debugging or verification)
void printAllObjects() {
    std::cout << "\n--- BANEN ---\n";
    for (const BAAN& baan : banen) {
        std::cout << "Baan: " << baan.naam << ", lengte: " << baan.lengte << std::endl;
    }

    std::cout << "\n--- VOERTUIGEN ---\n";
    for (const VOERTUIG& voertuig : voertuigen) {
        std::cout << "Voertuig op baan: " << voertuig.baan << ", positie: " << voertuig.positie << std::endl;
    }

    std::cout << "\n--- VERKEERSLICHTEN ---\n";
    for (const VERKEERSLICHT& verkeerslicht : verkeerslichten) {
        std::cout << "Verkeerslicht op baan: " << verkeerslicht.baan
                  << ", positie: " << verkeerslicht.positie
                  << ", cyclus: " << verkeerslicht.cyclus << std::endl;
    }

    std::cout << "\n--- VOERTUIGGENERATOREN ---\n";
    for (const VOERTUIGGENERATOR& generator : voertuiggeneratoren) {
        std::cout << "Generator op baan: " << generator.baan
                  << ", frequentie: " << generator.frequentie << std::endl;
    }
}

