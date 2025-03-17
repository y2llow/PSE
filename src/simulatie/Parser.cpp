//
// Created by s0246031@ad.ua.ac.be on 3/17/25.
//

#include "Parser.h"

#include <iostream>


void Parser::parseBanen(TiXmlElement *root, simulation *sim) {
    for (TiXmlElement *elem = root->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement()) {
        string elementType = elem->Value();

        if (elementType == "BAAN") {
            Baan *baan = new Baan();
            bool geldig = true;
            // Loop through sub-elements to get properties
            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                string propertyName = subElem->Value();

                if (propertyName == "naam") {
                    if (!subElem->GetText()) {
                        cerr << "Er is een baan zonder naam!" << endl;
                        geldig = false;
                        break;
                    }
                    baan->setNaam(subElem->GetText());
                } else if (propertyName == "lengte") {
                    if (!subElem->GetText()) {
                        cerr << "Er is een baan zonder lengte!" << endl;
                        geldig = false;
                        break;
                    }
                    try {
                        baan->setLengte(stoi(subElem->GetText()));
                    } catch (exception &) {
                        cerr << "De lengte van een baan is geen integer!" << endl;
                        geldig = false;
                        break;
                    }
                }
            }
            if (geldig) { sim->addBaan(baan); } // Add to vector
        }
    }
}

void Parser::parseOtherElements(TiXmlElement *root, simulation *sim) {
        for (TiXmlElement *elem = root->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement()) {
        string elementType = elem->Value();

        if (elementType == "VOERTUIG") {
            Voertuig *voertuig = new Voertuig();
            bool geldig = true;

            voertuig->setId(sim->getVoertuigLastId());
            sim->incSimulationTime();

            voertuig->setSnelheid(16.6); // TODO vw dit mischien
            //we geven vMax de waarde van Vmax
            voertuig->setKvmax(MAX_SNELHEID);

            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                string propertyName = subElem->Value();

                if (propertyName == "baan") {
                    if (!subElem->GetText()) {
                        cerr << "Er is een voertuig zonder baan!" << endl;
                        geldig = false;
                        break;
                    }

                    Baan* voertuigbaan = nullptr;
                    for (Baan* baan : sim->getBanen()) {
                        if (baan->getNaam() == subElem->GetText()) {
                            voertuigbaan = baan;
                            break;
                        }
                    }
                    voertuig->setBaan(voertuigbaan);

                } else if (propertyName == "snelheid") {
                    if (!subElem->GetText()) {
                        cerr << "Er is een voertuig zonder snelheid!" << endl;
                        geldig = false;
                        break;
                    }
                    string snelheidstring = subElem->GetText();

                    try {
                        double snelheid = std::stod(snelheidstring);
                        voertuig->setSnelheid(snelheid);
                        geldig = stoi(subElem->GetText()) >= 0; // snelheid moet positief zijn
                    } catch (exception &) {
                        cerr << "De snelheid van een voertuig is geen double!" << endl;
                        geldig = false;
                        break;
                    }
                } else if (propertyName == "positie") {
                    if (!subElem->GetText()) {
                        cerr << "Er is een voertuig zonder positie!" << endl;
                        geldig = false;
                        break;
                    }
                    try {
                        voertuig->setPositie(stoi(subElem->GetText()));
                        geldig = stoi(subElem->GetText()) >= 0; // positie moet positief zijn
                    } catch (exception &) {
                        cerr << "Er is een voertuig waarvan de positie geen integer is!" << endl;
                        geldig = false;
                        break;
                    }
                }
            }
            if (geldig) sim->addVoertuig(voertuig);
        } else if (elementType == "VERKEERSLICHT") {
            Verkeerslicht *verkeerslicht = new Verkeerslicht();
            bool geldig = true;

            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                string propertyName = subElem->Value();

                if (propertyName == "baan") {
                    if (!subElem->GetText()) {
                        cerr << "Er is een verkeerslicht zonder baan!" << endl;
                        geldig = false;
                        break;
                    }

                    Baan* voertuigbaan = nullptr;
                    for (Baan* baan : sim->getBanen()) {
                        if (baan->getNaam() == subElem->GetText()) {
                            voertuigbaan = baan;
                            break;
                        }
                    }
                    verkeerslicht->setBaan(voertuigbaan);
                } else if (propertyName == "positie") {
                    if (!subElem->GetText()) {
                        cerr << "Er is een verkeerslicht zonder positie!" << endl;
                        geldig = false;
                        break;
                    }
                    try {
                        verkeerslicht->setPositie(stoi(subElem->GetText()));
                        geldig = stoi(subElem->GetText()) >= 0; // Cyclus moet positief zijn
                    } catch (exception &) {
                        cerr << "Er is een verkeerslicht waarvan de positie geen integer is!" << endl;
                        geldig = false;
                        break;
                    }
                } else if (propertyName == "cyclus") {
                    if (!subElem->GetText()) {
                        cerr << "Er is een verkeerslicht zonder cyclus!" << endl;
                        geldig = false;
                        break;
                    }
                    try {
                        verkeerslicht->setCyclus(stoi(subElem->GetText()));
                        geldig = stoi(subElem->GetText()) >= 0; // Cyclus moet positief zijn
                    } catch (exception &) {
                        cerr << "Er is een verkeerslicht waarvan de cyclus geen integer is!" << endl;
                        geldig = false;
                        break;
                    }
                }
            }
            if (geldig) sim->addVerkeerslicht(verkeerslicht);
        } else if (elementType == "VOERTUIGGENERATOR") {
            Voertuiggenerator *generator = new Voertuiggenerator();
            bool geldig = true;

            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                string propertyName = subElem->Value();

                if (propertyName == "baan") {
                    if (!subElem->GetText()) {
                        cerr << "Er is een VOERTUIGGENERATOR zonder baan!" << endl;
                        geldig = false;
                        break;
                    }
                    Baan* voertuigbaan = nullptr;
                    for (Baan* baan : sim->getBanen()) {
                        if (baan->getNaam() == subElem->GetText()) {
                            voertuigbaan = baan;
                            break;
                        }
                    }
                    generator->setBaan(voertuigbaan);
                } else if (propertyName == "frequentie") {
                    if (!subElem->GetText()) {
                        cerr << "Er is een frequentie zonder baan!" << endl;
                        geldig = false;
                        break;
                    }

                    try {
                        int freq = stoi(subElem->GetText());
                        generator->setFrequentie(freq);
                        geldig = freq >= 0; // frequentie moet positief zijn
                    } catch (exception &) {
                        cerr << "Er is een verkeerslicht waarvan de cyclus geen integer is!" << endl;
                        geldig = false;
                        break;
                    }
                }
            }

            if (geldig) sim->addVoertuiggenerator(generator);
        } else {
            cerr << "Er is een onherkenbaar element in de XML bestand" << endl;
        }
    }

}

bool Parser::parseElements(const std::string &filename, simulation *sim) {
    TiXmlDocument doc;

    if (!doc.LoadFile(filename.c_str())) {
        cerr << "Error loading file: " << doc.ErrorDesc() << endl;
        return false;
    }

    TiXmlElement *root = doc.FirstChildElement();
    if (!root) {
        cerr << "Failed to load file: No root element." << endl;
        return true;
    }

    parseBanen(root, sim);
    parseOtherElements(root, sim);

    sim->sortVoertuigenByPosition();
    sim->sortVerkeersLichtByPosition();
    return true;
}
