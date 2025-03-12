#include <iostream>
#include <string>
#include "VerkeersSituatieInlezen.h"
#include "../../src/TinyXML/tinyxml.h"


// Function to parse XML and create appropriate objects
void VerkeerssituatieInlezen::parseXMLAndCreateObjects(const string &filename) {
    TiXmlDocument doc;
    if (!doc.LoadFile(filename.c_str())) {
        cerr << "Error loading file: " << doc.ErrorDesc() << endl;
        return;
    }

    TiXmlElement *root = doc.FirstChildElement();
    if (!root) {
        cerr << "Failed to load file: No root element." << endl;
        return;
    }

    for (TiXmlElement *elem = root->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement()) {
        string elementType = elem->Value();

        if (elementType == "BAAN") {
            Baan baan;
            bool geldig = true;
            // Loop through sub-elements to get properties
            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                string propertyName = subElem->Value();

                if (propertyName == "naam") {
                    if (!subElem->GetText()) {
                        cout << "Er is een baan zonder naam!" << endl;
                        geldig = false;
                        break;
                    }
                    baan.setNaam(subElem->GetText());
                } else if (propertyName == "lengte") {
                    if (!subElem->GetText()) {
                        cout << "Er is een baan zonder lengte!" << endl;
                        geldig = false;
                        break;
                    }
                    try {
                        baan.setLengte(stoi(subElem->GetText()));
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
                string propertyName = subElem->Value();

                if (propertyName == "baan") {
                    if (!subElem->GetText()) {
                        cout << "Er is een voertuig zonder baan!" << endl;
                        geldig = false;
                        break;
                    }
                    voertuig.setBaan(subElem->GetText());
                } else if (propertyName == "positie") {
                    if (!subElem->GetText()) {
                        cout << "Er is een voertuig zonder positie!" << endl;
                        geldig = false;
                        break;
                    }
                    try {
                        auto weird = subElem->GetText();
                        voertuig.setPositie(stoi(weird));
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
            bool geldig = true;

            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                string propertyName = subElem->Value();

                if (propertyName == "baan") {
                    if (!subElem->GetText()) {
                        cout << "Er is een verkeerslicht zonder baan!" << endl;
                        geldig = false;
                        break;
                    }
                    verkeerslicht.setBaan(subElem->GetText());
                } else if (propertyName == "positie") {
                    if (!subElem->GetText()) {
                        cout << "Er is een verkeerslicht zonder positie!" << endl;
                        geldig = false;
                        break;
                    }
                    try {
                        verkeerslicht.setPositie(stoi(subElem->GetText()));
                    } catch (exception &) {
                        cout << "Er is een verkeerslicht waarvan de positie geen integer is!" << endl;
                        geldig = false;
                        break;
                    }
                } else if (propertyName == "cyclus") {
                    if (!subElem->GetText()) {
                        cout << "Er is een verkeerslicht zonder cyclus!" << endl;
                        geldig = false;
                        break;
                    }
                    try {
                        verkeerslicht.setCyclus(stoi(subElem->GetText()));
                    } catch (exception &) {
                        cout << "Er is een verkeerslicht waarvan de cyclus geen integer is!" << endl;
                        geldig = false;
                        break;
                    }
                }
            }

            if (geldig)
                verkeerslichten.push_back(verkeerslicht);
        } else if (elementType == "VOERTUIGGENERATOR") {
            Voertuiggenerator generator;

            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                string propertyName = subElem->Value();

                if (propertyName == "baan" && subElem->GetText()) {
                    generator.setBaan(subElem->GetText());
                } else if (propertyName == "frequentie" && subElem->GetText()) {
                    generator.setFrequentie(stoi(subElem->GetText()));
                }
            }

            voertuiggeneratoren.push_back(generator);
        } else {
            cerr << "Er is een onherkenbaar element in de XML bestand" << endl;
        }
    }
}


vector<Baan> VerkeerssituatieInlezen::getBanen() const {
    return banen;
}

vector<Voertuig> VerkeerssituatieInlezen::getVoertuigen() const {
    return voertuigen;
}

vector<Verkeerslicht> VerkeerssituatieInlezen::getVerkeerslichten() const {
    return verkeerslichten;
}


vector<Voertuiggenerator> VerkeerssituatieInlezen::getVoertuiggeneratoren() const {
    return voertuiggeneratoren;
}
