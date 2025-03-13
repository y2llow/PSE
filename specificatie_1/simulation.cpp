//
// Created by eraya on 13/03/2025.
//

#include "simulation.h"
#include <iostream>
#include <map>
#include <string>
#include "simulation.h"
#include "../../src/TinyXML/tinyxml.h"


// Function to parse XML and create appropriate objects
void simulation::parseXMLAndCreateObjects(const string &filename) {
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

    int voertuigLastId = 0; // TODO pas dit aan zodat we dit ergens anders kunnen opslagen
    for (TiXmlElement *elem = root->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement()) {
        string elementType = elem->Value();

        if (elementType == "BAAN") {
            Baan* baan = new Baan();
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
                    baan->setNaam(subElem->GetText());
                } else if (propertyName == "lengte") {
                    if (!subElem->GetText()) {
                        cout << "Er is een baan zonder lengte!" << endl;
                        geldig = false;
                        break;
                    }
                    try {
                        baan->setLengte(stoi(subElem->GetText()));
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
            voertuigLastId ++;
            Voertuig* voertuig = new Voertuig();
            bool geldig = true;
            voertuig->setId(voertuigLastId);

            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                string propertyName = subElem->Value();

                if (propertyName == "baan") {
                    if (!subElem->GetText()) {
                        cout << "Er is een voertuig zonder baan!" << endl;
                        geldig = false;
                        break;
                    }
                    voertuig->setBaan(subElem->GetText());
                } else if (propertyName == "positie") {
                    if (!subElem->GetText()) {
                        cout << "Er is een voertuig zonder positie!" << endl;
                        geldig = false;
                        break;
                    }
                    try {
                        voertuig->setPositie(stoi(subElem->GetText()));
                    } catch (exception &) {
                        cout << "Er is een voertuig waarvan de positie geen integer is!" << endl;
                        geldig = false;
                        break;
                    }
                }
            }

            if (geldig) voertuigen.push_back(voertuig);
        } else if (elementType == "VERKEERSLICHT") {
            auto* verkeerslicht = new Verkeerslicht();
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
                    verkeerslicht->setBaan(subElem->GetText());
                } else if (propertyName == "positie") {
                    if (!subElem->GetText()) {
                        cout << "Er is een verkeerslicht zonder positie!" << endl;
                        geldig = false;
                        break;
                    }
                    try {
                        verkeerslicht->setPositie(stoi(subElem->GetText()));
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
                        verkeerslicht->setCyclus(stoi(subElem->GetText()));
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
            auto *generator = new Voertuiggenerator();

            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                string propertyName = subElem->Value();

                if (propertyName == "baan" && subElem->GetText()) {
                    generator->setBaan(subElem->GetText());
                } else if (propertyName == "frequentie" && subElem->GetText()) {
                    generator->setFrequentie(stoi(subElem->GetText()));
                }
            }

            voertuiggeneratoren.push_back(generator);
        } else {
            cerr << "Er is een onherkenbaar element in de XML bestand" << endl;
        }
    }
}


vector<Baan *> simulation::getBanen() const {
    return banen;
}

vector<Voertuig *> simulation::getVoertuigen() const {
    return voertuigen;
}

vector<Verkeerslicht *> simulation::getVerkeerslichten() const {
    return verkeerslichten;
}


vector<Voertuiggenerator *> simulation::getVoertuiggeneratoren() const {
    return voertuiggeneratoren;
}

bool simulation::isConsistent() const {
    bool consistent = true;

    // Elk voertuig staat op een bestaande baan
    for (Voertuig* const& v: voertuigen) {
        consistent = false;
        for (Baan* const& b: banen) {
            if (v->getBaan() == b->getNaam()) {
                consistent = true;
                break;
            }
        }
    }
    if (!consistent) return consistent;

    // Elk verkeerslicht staat op een bestaande baan.
    for (Verkeerslicht* const& v: verkeerslichten) {
        consistent = false;
        for (Baan* const& b: banen) {
            if (v->getBaan() == b->getNaam()) {
                consistent = true;
                break;
            }
        }
    }
    if (!consistent) return consistent;

    // Elke voertuiggenerator staat op een bestaande baan.
    for (Voertuiggenerator* const& v: voertuiggeneratoren) {
        consistent = false;
        for (Baan* const& b: banen) {
            if (v->getBaan() == b->getNaam()) {
                consistent = true;
                break;
            }
        }
    }
    if (!consistent) return consistent;

    // De positie van elk voertuig is kleiner dan de lengte van de baan.
    for (Voertuig* const& v: voertuigen) {
        if (getBaan(v->getBaan()) == nullptr || v->getPositie() > getBaan(v->getBaan())->getLengte()) {
            return false;
        }
    }

    // De positie van elk verkeerslicht is kleiner dan de lengte van de baan.
    for (Verkeerslicht* const& v: verkeerslichten) {
        if (getBaan(v->getBaan()) == nullptr || v->getPositie() > getBaan(v->getBaan())->getLengte()) {
            return false;
        }
    }

    // Er is maximaal ´e´en voertuiggenerator op elke baan.
    map<string, int> vg_op_banen;
    for (Baan* const& b : banen) {
        vg_op_banen[b->getNaam()] = 0;
    }

    for (Voertuiggenerator* const& v: voertuiggeneratoren) {
        vg_op_banen[v->getBaan()] += 1;
    }

    for (const auto& k : vg_op_banen) {
        if (k.second > 1) {
            return false;
        }
    }

    return true;
}

Baan *simulation::getBaan(const string &name) const {
    for (Baan* b: banen) {
        if (b->getNaam() == name) {
            return b;
        }
    }
    return nullptr;
}

void simulation::ToString() const {
    cout << "Tijd: 0"  << endl; //TODO tijd functie aanpassen

    for (Voertuig* voertuig : voertuigen) {
        cout << "Voertuig " << voertuig->getId() << endl;
        cout << "-> baan: " << voertuig->getBaan() << endl;
        cout << "-> positie: " << voertuig->getPositie() << endl;
        cout << "-> snelheid: " << voertuig->getSnelheid() << endl;
    }
}

