#include <iostream>
#include <string>
#include "Verkeerssituatie_inlezen.h"
#include "../../src/TinyXML/tinyxml.h"


// Function to parse XML and create appropriate objects
void Verkeerssituatie_inlezen::parseXMLAndCreateObjects(const std::string &filename) {
    TiXmlDocument doc;
    if (!doc.LoadFile(filename.c_str())) {
        std::cerr << "Error loading file: " << doc.ErrorDesc() << std::endl;
        return;
    }

    TiXmlElement *root = doc.FirstChildElement();
    if (!root) {
        std::cerr << "Failed to load file: No root element." << std::endl;
        return;
    }

    for (TiXmlElement *elem = root->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement()) {
        std::string elementType = elem->Value();

        if (elementType == "BAAN") {
            Baan baan;
            bool geldig = true;
            // Loop through sub-elements to get properties
            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                std::string propertyName = subElem->Value();

                if (propertyName == "naam") {
                    if (!subElem->GetText()) {
                        cout << "Er is een baan zonder naam!" << endl;
                        geldig = false;
                        break;
                    }
                    baan.naam = subElem->GetText();
                } else if (propertyName == "lengte") {
                    if (!subElem->GetText()) {
                        cout << "Er is een baan zonder lengte!" << endl;
                        geldig = false;
                        break;
                    }
                    try {
                        baan.lengte = std::stoi(subElem->GetText());
                    } catch (exception &) {
                        cout << "De lengte van een baan is geen integer!" << endl;
                        geldig = false;
                        break;
                    }
                }
            }
            // Add to vector
            if (geldig) banen.push_back(baan);
        } else if (elementType == "VOERTUIG") {
            Voertuig voertuig;
            bool geldig = true;

            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                std::string propertyName = subElem->Value();

                if (propertyName == "baan") {
                    if (!subElem->GetText()) {
                        cout << "Er is een voertuig zonder baan!" << endl;
                        geldig = false;
                        break;
                    }
                    voertuig.baan = subElem->GetText();
                } else if (propertyName == "positie") {
                    if (!subElem->GetText()) {
                        cout << "Er is een voertuig zonder positie!" << endl;
                        geldig = false;
                        break;
                    }
                    try {
                        voertuig.positie = std::stoi(subElem->GetText());
                    } catch (exception &) {
                        cout << "Er is een voertuig waarvan de positie geen integer is!" << endl;
                        geldig = false;
                        break;
                    }
                }
            }

            if (geldig) voertuigen.push_back(voertuig);
        } else if (elementType == "VERKEERSLICHT") {
            Verkeerslicht verkeerslicht;

            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
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
        } else if (elementType == "VOERTUIGGENERATOR") {
            Voertuiggenerator generator;

            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                std::string propertyName = subElem->Value();

                if (propertyName == "baan" && subElem->GetText()) {
                    generator.baan = subElem->GetText();
                } else if (propertyName == "frequentie" && subElem->GetText()) {
                    generator.frequentie = std::stoi(subElem->GetText());
                }
            }

            voertuiggeneratoren.push_back(generator);
        } else {
            cerr << "Er is een onherkenbaar element in de XML bestand" << endl;
        }
    }
}


std::vector<Baan> Verkeerssituatie_inlezen::get_banen() const {
    return banen;
}

std::vector<Voertuig> Verkeerssituatie_inlezen::get_voertuigen() const {
    return voertuigen;
}

std::vector<Verkeerslicht> Verkeerssituatie_inlezen::get_verkeerslichten() const {
    return verkeerslichten;
}


std::vector<Voertuiggenerator> Verkeerssituatie_inlezen::get_voertuiggeneratoren() const {
    return voertuiggeneratoren;
}
