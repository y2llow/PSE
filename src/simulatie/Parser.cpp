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
            // Standaard type is AUTO
            VoertuigType type = VoertuigType::AUTO;

            // Eerst controleren we of er een type is gespecificeerd
            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                string propertyName = subElem->Value();
                if (propertyName == "type") {
                    if (subElem->GetText()) {
                        std::string typeString = subElem->GetText();

                        // Converteer string naar VoertuigType enum
                        if (typeString == "auto") {
                            type = VoertuigType::AUTO;
                        } else if (typeString == "bus") {
                            type = VoertuigType::BUS;
                        } else if (typeString == "brandweerwagen") {
                            type = VoertuigType::BRANDWEERWAGEN;
                        } else if (typeString == "ziekenwagen") {
                            type = VoertuigType::ZIEKENWAGEN;
                        } else if (typeString == "politiecombi") {
                            type = VoertuigType::POLITIECOMBI;
                        } else {
                            cerr << "Onbekend voertuigtype: " << typeString << ". Type AUTO wordt gebruikt." << endl;
                        }
                    }
                    break;
                }
            }

            // Nu we het type weten, maken we het voertuig
            Baan *voertuigbaan = nullptr;
            int positie = 0;
            bool geldig = true;

            // Eerste pass om baan en positie te verzamelen (nodig voor constructor)
            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                string propertyName = subElem->Value();

                if (propertyName == "baan") {
                    if (!subElem->GetText()) {
                        cerr << "Er is een voertuig zonder baan!" << endl;
                        geldig = false;
                        break;
                    }

                    for (Baan *baan: sim->getBanen()) {
                        if (baan->getNaam() == subElem->GetText()) {
                            voertuigbaan = baan;
                            break;
                        }
                    }

                    if (voertuigbaan == nullptr) {
                        cerr << "Baan niet gevonden voor voertuig!" << endl;
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
                        positie = stoi(subElem->GetText());
                        if (positie < 0) {
                            cerr << "Positie moet positief zijn!" << endl;
                            geldig = false;
                            break;
                        }
                    } catch (exception &) {
                        cerr << "Er is een voertuig waarvan de positie geen integer is!" << endl;
                        geldig = false;
                        break;
                    }
                }
            }

            // Als we geen geldige baan of positie hebben, sla over
            if (!geldig || voertuigbaan == nullptr) {
                cerr << "Voertuig wordt overgeslagen vanwege ongeldige baan of positie." << endl;
                return; // of continue als dit in een lus staat
            }

            // Maak het voertuig met het juiste type
            Voertuig *voertuig = new Voertuig(voertuigbaan, positie, type);
            voertuig->setId(sim->getVoertuigLastId());
            sim->increaseVoertuigLastId();

            // Initialiseer de snelheid met de maximale snelheid van het type
            voertuig->setSnelheid(voertuig->getMaxSnelheid());
            voertuig->setKvmax(voertuig->getMaxSnelheid());

            // Tweede pass voor andere eigenschappen (zoals snelheid)
            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                string propertyName = subElem->Value();

                // Sla baan, positie en type over, die hebben we al verwerkt
                if (propertyName == "baan" || propertyName == "positie" || propertyName == "type") {
                    continue;
                } else if (propertyName == "snelheid") {
                    if (!subElem->GetText()) {
                        cerr << "Er is een voertuig zonder snelheid!" << endl;
                        geldig = false;
                        break;
                    }
                    string snelheidstring = subElem->GetText();

                    try {
                        double snelheid = std::stod(snelheidstring);
                        if (snelheid < 0) {
                            cerr << "Snelheid moet positief zijn!" << endl;
                            geldig = false;
                            break;
                        }
                        voertuig->setSnelheid(snelheid);
                    } catch (exception &) {
                        cerr << "De snelheid van een voertuig is geen double!" << endl;
                        geldig = false;
                        break;
                    }
                }
                // Hier kun je meer eigenschappen toevoegen indien nodig
            }

            // Voeg het voertuig toe aan de simulatie als het geldig is
            if (geldig) {
                sim->addVoertuig(voertuig);
            } else {
                delete voertuig; // Voorkom memory leak als het voertuig ongeldig is
            }
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

                    Baan *voertuigbaan = nullptr;
                    for (Baan *baan: sim->getBanen()) {
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

                    Baan *voertuigbaan = nullptr;
                    for (Baan *baan: sim->getBanen()) {
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
            // Standaard waarden
            Baan* voertuigbaan = nullptr;
            double frequentie = 0;
            VoertuigType type = VoertuigType::AUTO;
            bool geldig = true;

            // Verzamel alle benodigde eigenschappen
            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                string propertyName = subElem->Value();

                if (propertyName == "baan") {
                    if (!subElem->GetText()) {
                        cerr << "Er is een VOERTUIGGENERATOR zonder baan!" << endl;
                        geldig = false;
                        break;
                    }

                    for (Baan* baan : sim->getBanen()) {
                        if (baan->getNaam() == subElem->GetText()) {
                            voertuigbaan = baan;
                            break;
                        }
                    }

                    if (voertuigbaan == nullptr) {
                        cerr << "Baan niet gevonden voor voertuiggenerator!" << endl;
                        geldig = false;
                        break;
                    }
                }
                else if (propertyName == "frequentie") {
                    if (!subElem->GetText()) {
                        cerr << "Er is een voertuiggenerator zonder frequentie!" << endl;
                        geldig = false;
                        break;
                    }

                    try {
                        frequentie = stod(subElem->GetText());
                        if (frequentie < 0) {
                            cerr << "Frequentie moet positief zijn!" << endl;
                            geldig = false;
                            break;
                        }
                    } catch (exception &) {
                        cerr << "De frequentie van een voertuiggenerator is geen getal!" << endl;
                        geldig = false;
                        break;
                    }
                }
                else if (propertyName == "type") {
                    if (subElem->GetText()) {
                        std::string typeString = subElem->GetText();

                        // Converteer string naar VoertuigType enum
                        // Converteer string naar VoertuigType enum
                        if (typeString == "auto") {
                            type = VoertuigType::AUTO;
                        } else if (typeString == "bus") {
                            type = VoertuigType::BUS;
                        } else if (typeString == "brandweerwagen") {
                            type = VoertuigType::BRANDWEERWAGEN;
                        } else if (typeString == "ziekenwagen") {
                            type = VoertuigType::ZIEKENWAGEN;
                        } else if (typeString == "politiecombi") {
                            type = VoertuigType::POLITIECOMBI;
                        } else {
                            cerr << "Onbekend voertuigtype: " << typeString << ". Type AUTO wordt gebruikt." << endl;
                        }
                    }
                }
            }

            // Controleer of alle verplichte eigenschappen zijn ingesteld
            if (!geldig || voertuigbaan == nullptr) {
                cerr << "Voertuiggenerator wordt overgeslagen vanwege ongeldige parameters." << endl;
                return; // of continue als dit in een lus staat
            }

            // Maak de generator met de verzamelde gegevens
            Voertuiggenerator* generator = new Voertuiggenerator(voertuigbaan, frequentie, type);

            // Voeg de generator toe aan de simulatie
            sim->addVoertuiggenerator(generator);
        } else if (elementType == "BAAN"){
            continue;
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
