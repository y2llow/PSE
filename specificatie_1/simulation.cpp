//
// Created by eraya on 13/03/2025.
//

#include "simulation.h"
#include "../src/elementen/Constants.h"
#include <iostream>
#include <map>
#include <string>
#include "simulation.h"
#include "../../src/TinyXML/tinyxml.h"
#include <algorithm>
#include <cmath>


// Function to parse XML and create appropriate objects
bool simulation::parseXMLAndCreateObjects(const string &filename) {
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
            if (geldig) { banen.push_back(baan); } // Add to vector
        } else if (elementType == "VOERTUIG") {
            Voertuig *voertuig = new Voertuig();
            bool geldig = true;

            voertuig->setId(voertuigLastId);
            voertuigLastId++;

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

                    // Dit kan tot problemen en errors lijden als de voertuigen zijn eerst gedefinierd in de xml bestand voor de Baan
                    // Baan voertuigbaan;
                    // for (Baan* baan : banen) {
                    //     if (baan->getNaam() == subElem->GetText()) {
                    //         voertuigbaan.setNaam(baan->getNaam());
                    //         voertuigbaan.setLengte(baan->getLengte());
                    //     }
                    // }

                    voertuig->setBaan(subElem->GetText());
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
            if (geldig) voertuigen.push_back(voertuig);
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

                    // Dit leidt tot problemen als het verkeerslicht gedefinieerd is voor de Baan in de xml file
                    // Baan Verkeerslichtbaan;
                    // for (Baan * baan : banen) {
                    //     if (baan->getNaam() == subElem->GetText()) {
                    //         Verkeerslichtbaan.setNaam(baan->getNaam());
                    //         Verkeerslichtbaan.setLengte(baan->getLengte());
                    //     }
                    // }
                    verkeerslicht->setBaan(subElem->GetText());
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
            if (geldig) verkeerslichten.push_back(verkeerslicht);
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
                    generator->setBaan(subElem->GetText());
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

            if (geldig) voertuiggeneratoren.push_back(generator);
        } else {
            cerr << "Er is een onherkenbaar element in de XML bestand" << endl;
        }
    }

    sortVoertuigenByPosition();
    sortVerkeersLichtByPosition();
    return true;
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
    // Empty file
    if (voertuigen.empty() && verkeerslichten.empty() && voertuiggeneratoren.empty() && banen.empty())
        return false;

    // Elk voertuig staat op een bestaande baan
    for (Voertuig *const&v: voertuigen) {
        if (getBaanByName(v->getBaan()) == nullptr) return false;
    }

    // Elk verkeerslicht staat op een bestaande baan
    for (Verkeerslicht *const&v: verkeerslichten) {
        if (getBaanByName(v->getBaan()) == nullptr) return false;
    }

    // Elke voertuiggenerator staat op een bestaande baan
    for (Voertuiggenerator *const&v: voertuiggeneratoren) {
        if (getBaanByName(v->getBaan()) == nullptr) return false;
    }

    // De positie van elk voertuig is kleiner dan de lengte van de baan
    for (Voertuig *const&v: voertuigen) {
        if (v->getPositie() > getBaanByName(v->getBaan())->getLengte()) {
            return false;
        }
    }

    // De positie van elk verkeerslicht is kleiner dan de lengte van de baan
    for (Verkeerslicht *const&v: verkeerslichten) {
        if (v->getPositie() > getBaanByName(v->getBaan())->getLengte()) {
            return false;
        }
    }

    // Er is maximaal één voertuiggenerator op elke baan
    map<string, int> vg_op_banen;
    for (Baan *const&b: banen) {
        vg_op_banen[b->getNaam()] = 0;
    }

    for (Voertuiggenerator *const&v: voertuiggeneratoren) {
        vg_op_banen[v->getBaan()] += 1;
    }

    for (const pair<const string, int> &k: vg_op_banen) {
        if (k.second > 1) {
            return false;
        }
    }

    return true;
}

void simulation::ToString() {
    cout << "------------- SIMULATIE " << getSimulationTime() << " ------------- " << endl;

    cout << "Tijd: " << getincSimulationTime() << endl;

    for (Voertuig *voertuig: voertuigen) {
        cout << "Voertuig " << voertuig->getId() << "\n"
                << "-> baan: " << voertuig->getBaan() << "\n"
                << "-> positie: " << voertuig->getPositie() << "\n"
                << "-> snelheid: " << voertuig->getSnelheid() << "\n" << endl;
    }
}

double simulation::getSimulationTime() const {
    return simulationTime;
}

double simulation::getincSimulationTime() const {
    return simulationincreasedTime;
}

double simulation::incSimulationTime() {
    simulationincreasedTime = simulationincreasedTime + SIMULATIE_TIJD;
    return simulationincreasedTime;
}

double simulation::UpdateSimulationTime() const {
    return simulationTime + SIMULATIE_TIJD;
    //TODO fix updatesimulationtime so that it updates somewhere in the program
}


void simulation::sortVoertuigenByPosition() {
    sort(voertuigen.begin(), voertuigen.end(), [](const Voertuig *a, const Voertuig *b) {
        return a->getPositie() < b->getPositie();
    });
}

void simulation::sortVerkeersLichtByPosition() {
    sort(verkeerslichten.begin(), verkeerslichten.end(), [](const Verkeerslicht *a, const Verkeerslicht *b) {
        return a->getPositie() < b->getPositie();
    });
}


void simulation::berekenPositie(Voertuig *v) const {
    //berekenen van nieuwe positie
    const double e = v->getSnelheid() + v->getVersnelling() * SIMULATIE_TIJD;
    double new_speed;
    double new_position;

    if (e < 0) {
        new_position = v->getPositie() - (v->getSnelheid() * v->getSnelheid()) / (2 * v->getVersnelling());
        new_speed = 0;
    } else {
        new_speed = v->getSnelheid() + v->getVersnelling() * SIMULATIE_TIJD;
        new_position = v->getPositie() + new_speed * SIMULATIE_TIJD + v->getVersnelling() * pow(SIMULATIE_TIJD, 2) / 2;
    }

    v->setSnelheid(new_speed);
    v->setPositie(new_position);
}

void simulation::BerekenVersnelling(Voertuig *v, int counter) const {
    double delta = 0;

    size_t vsize = voertuigen.size();
    int sizeVoertuigen = vsize;

    if (sizeVoertuigen > counter + 1) {
        double volgafstand = voertuigen[counter + 1]->getPositie() - v->getPositie() - LENGTE;
        double snelheidVerschil = v->getSnelheid() - voertuigen[counter + 1]->getSnelheid();

        double newsnelheid = v->getSnelheid() - snelheidVerschil;
        double newversnelling = 2 * sqrt(MAX_VERSNELLING * MAX_REMFACTOR);

        double calculate = v->getSnelheid() + (newsnelheid / newversnelling);

        double maxNummer = max(0.0, calculate);
        delta = (MIN_VOLGAFSTAND + maxNummer) / volgafstand;

        double newVersnelling = MAX_VERSNELLING * (1 - pow((v->getSnelheid() / v->getKvmax()), 4) -
                                                   pow(delta, 2));
        v->setVersnelling(newVersnelling);
    } else {
        double newVersnelling = MAX_VERSNELLING * (1 - pow((v->getSnelheid() / v->getKvmax()), 4) -
                                                   pow(delta, 2));
        v->setVersnelling(newVersnelling);
    }
}

void simulation::updateVoertuig(Voertuig *v, int counter) const {
    berekenPositie(v);
    //TODO: als de voertuig in stop/vertraagzone zit dan moet die vertragen in plaats van versnellen.
    //TODO: vind een manier om de IsVoertuigInStopZone en isVoertuigInVertraagZone hier te laten werken.
    BerekenVersnelling(v, counter);
}

bool simulation::isVoertuigOpBaan(const Voertuig *v) {
    Baan *baan = getBaanByName(v->getBaan()); // de baan van de voertuig

    return v->getPositie() <= baan->getLengte();
}

void simulation::simulationRun() {
    int counter = 0;

    for (Voertuig *v: voertuigen) {
        // 3.1 Updated voertuig positie en snelheid
        updateVoertuig(v, counter);

        if (!isVoertuigOpBaan(v)) {
            // Vehicle is no longer on the road, so remove it from the vector and delete it
            __gnu_cxx::__normal_iterator<Voertuig **, vector<Voertuig *> > it = find(
                voertuigen.begin(), voertuigen.end(), v); // Find the iterator for v

            if (it != voertuigen.end()) {
                // Ensure the element was found
                voertuigen.erase(it); // Remove the element from the vector
                delete v; // Free the memory
            }
        }

        counter++;
    }

    int verkeerslichtCounter = 0;
    for (Verkeerslicht *l: verkeerslichten) {
        // 3.2 Updated voertuig snelheid aan verkeerslicht situatie
        updateVoertuigAanVerkeerslichtSituatie(l, verkeerslichtCounter);
        verkeerslichtCounter++;
    }

    simulationTime++;
    incSimulationTime();

    // 3.4. Simulatie met voertuiggenerator
    voertuigenGenereren();
}

bool simulation::isVoertuigInVertraagZone(Voertuig *v, Verkeerslicht *l) {
    return v->getPositie() - l->getPositie() <= VERTRAAG_AFSTAND;
}

bool simulation::isVoertuigInStopZone(Voertuig *v, Verkeerslicht *l) {
    return v->getPositie() < l->getPositie() - STOP_AFSTAND / 2;
}

void simulation::BerekenSnelheidNaVertraging(Voertuig *v) {
    v->setKvmax(MAX_SNELHEID * VERTRAAG_FACTOR);
}

void simulation::BerekenSnelheidNaVersnelling(Voertuig *v) {
    v->setKvmax(MAX_SNELHEID);
}

vector<Voertuig *> simulation::voertuigenTussenVerkeerslichten(Verkeerslicht *lichtVoor, Verkeerslicht *lichtAchter) {
    vector<Voertuig *> VoertuigenVoorVerkeerslicht;

    // Als lichtVoor het laatste licht is, voeg gewoon alle auto's erachter aan de vector toe
    if (lichtAchter == nullptr) {
        for (Voertuig *v: voertuigen) {
            // Voeg alle voertuigen op dezelfde baan toe
            if (v->getBaan() == lichtVoor->getBaan()) {
                VoertuigenVoorVerkeerslicht.push_back(v);
            }
        }
    } else {
        // Voeg voertuigen toe die op dezelfde baan zijn en tussen de twee lichten staan
        for (Voertuig *v: voertuigen) {
            if (v->getBaan() == lichtVoor->getBaan() &&
                v->getPositie() > lichtAchter->getPositie() &&
                v->getPositie() < lichtVoor->getPositie()) {
                VoertuigenVoorVerkeerslicht.push_back(v);
            }
        }
    }

    return VoertuigenVoorVerkeerslicht;
}

vector<Verkeerslicht *> simulation::verkeerslichtenOpBaan(Verkeerslicht *licht) {
    vector<Verkeerslicht *> verkeersLichtenOpDezelfdeBaan;
    for (Verkeerslicht *v: verkeerslichten) {
        if (licht->getBaan() == v->getBaan()) {
            verkeersLichtenOpDezelfdeBaan.push_back(v);
        }
    }
    return verkeersLichtenOpDezelfdeBaan;
}

Baan *simulation::getBaanByName(const string &name) const {
    for (Baan *b: banen) {
        if (b->getNaam() == name) {
            return b;
        }
    }
    return nullptr;
}

void simulation::voertuigenGenereren() {
    // 3. FOR elke voertuiggenerator
    for (const Voertuiggenerator* v : voertuiggeneratoren) {
        string baan = v->getBaan();

        // 3.1 IF tijd sinds laatste voertuig > frequentie
        if (simulationTime - lastGeneretedVoertuigTime > v->getFrequentie()) {
            bool addCar = true;

            for (const Voertuig* voertuig : voertuigen) {
                if (baan == voertuig->getBaan() && voertuig->getPositie() >= 0 && voertuig->getPositie() <= 2) {
                    addCar = false;
                    break;
                }
            }

            // 3.1.1 IF geen voertuig op baan tussen posities 0 en 2l
            if (addCar) {
                Voertuig* generated_v = new Voertuig();

                // 3.1.1.1 THEN voeg voertuig toe aan baan op positie 0
                generated_v->setPositie(0);
                generated_v->setBaan(baan);
                generated_v->setId(voertuigLastId);
                generated_v->setKvmax(MAX_SNELHEID);

                voertuigLastId++;

                voertuigen.push_back(generated_v);

                lastGeneretedVoertuigTime = simulationTime;

                // sortVoertuigenByPosition();
            }
        }

    }
}


void simulation::updateVoertuigAanVerkeerslichtSituatie(Verkeerslicht *licht, int verkeerslichtCounter) {
    //  1. IF tijd sinds laatste verandering > cyclus
    licht->updateTijdSindsLaatsteVerandering(SIMULATIE_TIJD);

    if(licht->getTijdSindsLaatsteVerandering() > licht->getCyclus()){
        licht->updateVerkeersLicht(); // 1.1 THEN verander de kleur van het licht (groen ↔ rood)
    }

    vector<Verkeerslicht *> verkeerslichten_baan = verkeerslichtenOpBaan(licht);

    // Check of er nog een licht is op dezelfde straat
    Verkeerslicht *volgendeLicht = nullptr;


    if (static_cast<size_t>(verkeerslichtCounter) + 1 < verkeerslichten_baan.size()) {
        volgendeLicht = verkeerslichten_baan[verkeerslichtCounter + 1];
    }

    // Pak alle voertuigen tussen de lichten
    vector<Voertuig *> VoertuigenVoorLicht = voertuigenTussenVerkeerslichten(licht, volgendeLicht);

    // Pas alleen elementen aan voor eerste auto voor licht, de rest volgt automatisch
    if (!VoertuigenVoorLicht.empty()) {
        Voertuig *eerstVoertuigVoorLicht = VoertuigenVoorLicht.at(0);
        // pas alleen elementen aan voor eerste auto voor licht de rest volgt automtisch

        if (licht->isGroen()) {
            // 2. IF verkeerslicht is groen
            BerekenSnelheidNaVersnelling(eerstVoertuigVoorLicht);
            // 2.1 THEN voertuigen voor het verkeerslicht mag terug versnellen
        } else if (licht->isRood()) {
            // 3.1 ELSE IF verkeerslicht is rood
            if (isVoertuigInVertraagZone(eerstVoertuigVoorLicht, licht)) {
                // 3.1.1 THEN IF het eerste voertuig voor het licht bevindt zich in de vertraagafstand
                BerekenSnelheidNaVertraging(eerstVoertuigVoorLicht);
                // 3.1.1.1 THEN pas de vertraagfactor toe op het voertuig
            } else if (isVoertuigInStopZone(eerstVoertuigVoorLicht, licht)) {
                // 3.1.2 ELSE IF het eerste voertuig voor het licht bevindt zich in de eerste helft van de stopafstand
                eerstVoertuigVoorLicht->UpdateVersnellingVoorStoppen(); // 3.1.1.1 THEN laat het voertuig stoppen
            }
        }
    }
}
