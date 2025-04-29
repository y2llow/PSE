//
// Created by eraya on 13/03/2025.
//

#include "simulation.h"
#include "../elementen/Constants.h"
#include <iostream>
#include <map>
#include <string>
#include "../../src/TinyXML/tinyxml.h"
#include <algorithm>
#include <cmath>
#include <regex>

#include "SimPrinter.h"

simulation::simulation() {
    simulationTime = 0;
    simulationincreasedTime = 0;
    lastGeneretedVoertuigTime = 0;
    voertuigLastId = 1;
}

int simulation::getVoertuigLastId() const {
    return voertuigLastId;
}

void simulation::increaseVoertuigLastId() {
    voertuigLastId++;
}

// SimPrinter * simulation::getSimPrinter() const {
//     return simPrinter;
// }
//
// void simulation::setSimPrinter(SimPrinter * const sim_printer) {
//     simPrinter = sim_printer;
// }


vector<Baan *> simulation::getBanen() const {
    // Don't return the original vector, return a new copy with the same values so that the original one cannot get edited
    vector<Baan*> bb;
    for (auto* b : banen) {
        bb.push_back(b);
    }
    return bb;
}

vector<Voertuig *> simulation::getVoertuigen() const {
    // Don't return the original vector, return a new copy with the same values so that the original one cannot get edited
    vector<Voertuig*> vv;
    for (auto* b : voertuigen) {
        vv.push_back(b);
    }
    return vv;
}

vector<Verkeerslicht *> simulation::getVerkeerslichten() const {
    // Don't return the original vector, return a new copy with the same values so that the original one cannot get edited
    vector<Verkeerslicht*> vv;
    for (auto* b : verkeerslichten) {
        vv.push_back(b);
    }
    return vv;
}

vector<Bushalte *> simulation::getBushaltes() const {
    // Don't return the original vector, return a new copy with the same values so that the original one cannot get edited
    vector<Bushalte*> bb;
    for (auto* b : bushaltes) {
        bb.push_back(b);
    }
    return bb;
}

vector<Voertuiggenerator *> simulation::getVoertuiggeneratoren() const {
    // Don't return the original vector, return a new copy with the same values so that the original one cannot get edited
    vector<Voertuiggenerator*> vv;
    for (auto* b : voertuiggeneratoren) {
        vv.push_back(b);
    }
    return vv;
}

bool simulation::isConsistent() const {
    // Empty file
    if (voertuigen.empty() && verkeerslichten.empty() && voertuiggeneratoren.empty() && banen.empty())
        return false;

    // Elk voertuig staat op een bestaande baan
    for (Voertuig *const&v: voertuigen) {
        if (v->getBaan() == nullptr) return false;
    }

    // Elk verkeerslicht staat op een bestaande baan
    for (Verkeerslicht *const&v: verkeerslichten) {
        if (v->getBaan() == nullptr) return false;
    }

    // Elke voertuiggenerator staat op een bestaande baan
    for (Voertuiggenerator *const&v: voertuiggeneratoren) {
        if (v->getBaan() == nullptr) return false;
    }

    // De positie van elk voertuig is kleiner dan de lengte van de baan
    for (Voertuig *const&v: voertuigen) {
        if (v->getPositie() > v->getBaan()->getLengte()) {
            return false;
        }
    }

    // De positie van elk verkeerslicht is kleiner dan de lengte van de baan
    for (Verkeerslicht *const&v: verkeerslichten) {
        if (v->getPositie() > v->getBaan()->getLengte()) {
            return false;
        }
    }

    // Er is maximaal één voertuiggenerator op elke baan
    map<string, int> vg_op_banen;
    for (Baan *const&b: banen) {
        vg_op_banen[b->getNaam()] = 0;
    }

    for (Voertuiggenerator *const&v: voertuiggeneratoren) {
        vg_op_banen[v->getBaan()->getNaam()] += 1;
    }

    for (const pair<const string, int> &k: vg_op_banen) {
        if (k.second > 1) {
            return false;
        }
    }

    return true;
}

void simulation::ToString() {
    for (Voertuig *voertuig: voertuigen) {
        SimPrinter::printStatus(voertuig, getincSimulationTime());

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
            double volgafstand = voertuigen[counter + 1]->getPositie() - v->getPositie() - v->getLength();
            double snelheidVerschil = v->getSnelheid() - voertuigen[counter + 1]->getSnelheid();

            double newsnelheid = v->getSnelheid() * snelheidVerschil;
            double newversnelling = 2 * sqrt(v->getMaxVersnelling() * v->getMaxRemfactor());

            double calculate = v->getSnelheid() + (newsnelheid / newversnelling);

            double maxNummer = max(0.0, calculate);
            delta = (v->getMinVolgafstand() + maxNummer) / volgafstand;

            double newVersnelling = v->getMaxVersnelling() * (1 - pow((v->getSnelheid() / v->getKvmax()), 4) - pow(delta, 2));
            v->setVersnelling(newVersnelling);
        } else {
            double newVersnelling = v->getVersnelling() * (1 - pow((v->getSnelheid() / v->getKvmax()), 4) -
                                                       pow(delta, 2));
            v->setVersnelling(newVersnelling);
        }
    }


void simulation::updateVoertuig(Voertuig *v, int counter) const {
    berekenPositie(v);
    BerekenVersnelling(v, counter);
}

bool simulation::isVoertuigOpBaan(const Voertuig *v) {
    // Baan *baan = getBaanByName(v->getBaan()); TODO:// de baan van de voertuig

    Baan* baan = v->getBaan();

    return v->getPositie() <= baan->getLengte();
}

void simulation::simulationRun() {
    int counter = 0;

    // 3.4. Simulatie met voertuiggenerator
    voertuigenGenereren();

    for (Voertuig *v: voertuigen) {
        // TODO: make the program work this way
        // for voertuig:
        //    voertuig.simulate();

        // 3.1 Updated voertuig positie en snelheid
        updateVoertuig(v, counter);

        if (!isVoertuigOpBaan(v)) {
            // Vehicle is no longer on the road, so remove it from the vector and delete it
            auto it = find(
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
}

bool simulation::isVoertuigInVertraagZone(Voertuig *v, Verkeerslicht *l) {
    // Bepaal of het voertuig binnen de vertragingsafstand is
    bool isInVertraagZone = abs(v->getPositie() - l->getPositie()) <= VERTRAAG_AFSTAND;

    // Bepaal of het voertuig buiten de stopzone is
    bool isOutsideStopZone = v->getPositie() < l->getPositie() - STOP_AFSTAND;

    // Het voertuig is in de vertragingszone als het binnen de vertragingsafstand is en buiten de stopzone
    return isInVertraagZone && isOutsideStopZone;
}

bool simulation::isVoertuigInStopZone(Voertuig *v, Verkeerslicht *l) {
    bool isInStopZone1 = v->getPositie() < l->getPositie() - STOP_AFSTAND / 2;
    bool isInStopZone2 = v->getPositie() >= l->getPositie() - STOP_AFSTAND;

    return isInStopZone1 == true && true == isInStopZone2;
}

void simulation::BerekenSnelheidNaVertraging(Voertuig *v) {
    v->setKvmax(v->getMaxSnelheid() * VERTRAAG_FACTOR);
}

void simulation::BerekenSnelheidNaVersnelling(Voertuig *v) {
    v->setKvmax(v->getMaxSnelheid());
}

vector<Voertuig *> simulation::voertuigenTussenVerkeerslichten(Verkeerslicht *lichtVoor, Verkeerslicht *lichtAchter) {
    vector<Voertuig *> VoertuigenVoorVerkeerslicht;

    // Als lichtVoor het laatste licht is, voeg gewoon alle auto's erachter aan de vector toe
    if (lichtAchter == nullptr) {
        for (Voertuig *v: voertuigen) {
            // Voeg alle voertuigen op dezelfde baan toe
            if (v->getBaan() == lichtVoor->getBaan() && v->getPositie() < lichtVoor->getPositie()) {
                VoertuigenVoorVerkeerslicht.push_back(v);
            }
        }
    } else {
        // Voeg voertuigen toe die op dezelfde baan zijn en tussen de twee lichten staan
        for (Voertuig *v: voertuigen) {
            if (v->getBaan()->getNaam() == lichtVoor->getBaan()->getNaam() &&
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

void simulation::addVoertuig(Voertuig *v) {
    voertuigen.push_back(v);
}

void simulation::addVerkeerslicht(Verkeerslicht *v) {
    verkeerslichten.push_back(v);
}

void simulation::addBushalte(Bushalte *b) {
    bushaltes.push_back(b);
}

void simulation::addBaan(Baan *b) {
    banen.push_back(b);
}

void simulation::addVoertuiggenerator(Voertuiggenerator *v) {
    voertuiggeneratoren.push_back(v);
}

void simulation::voertuigenGenereren() {
    // 3. FOR elke voertuiggenerator
    for (const Voertuiggenerator* v : voertuiggeneratoren) {
        Baan* baan = v->getBaan();

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
                // Haal het type op uit de generator
                VoertuigType type = v->getType();

                // Maak een nieuw voertuig met het juiste type
                Voertuig* generated_v = new Voertuig(baan, 0, type);

                // Zet de nodige attributen
                generated_v->setId(voertuigLastId);
                generated_v->setSnelheid(generated_v->getMaxSnelheid()); // Gebruik de maximale snelheid van dit type
                generated_v->setKvmax(generated_v->getMaxSnelheid());    // Gebruik de maximale snelheid van dit type

                voertuigLastId++;

                voertuigen.push_back(generated_v);
                sortVoertuigenByPosition();

                lastGeneretedVoertuigTime = simulationTime;


                //2.2 voegen aan de simulatiePrinter
                char c ='X';
                if (type == VoertuigType::AUTO) {
                    c ='A';
                } else if (type == VoertuigType::BUS) {
                    c ='B';
                } else if (type == VoertuigType::BRANDWEERWAGEN) {
                    c ='I';
                } else if (type == VoertuigType::ZIEKENWAGEN) {
                    c ='Z';
                }else if (type == VoertuigType::POLITIECOMBI) {
                    c ='P';
                }
                simPr.addNewGeneratedVoertuigen(baan, c, generated_v);
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
        Voertuig *eerstVoertuigVoorLicht = VoertuigenVoorLicht.back();
        // pas alleen elementen aan voor eerste auto voor licht de rest volgt automtisch

        if (licht->isGroen()) {
            // 2. IF verkeerslicht is groen
            BerekenSnelheidNaVersnelling(eerstVoertuigVoorLicht);
            // 2.1 THEN voertuigen voor het verkeerslicht mag terug versnellen
        }if (licht->isRood()) {
            if (eerstVoertuigVoorLicht->getPrioriteit()) {
                BerekenSnelheidNaVersnelling(eerstVoertuigVoorLicht);
                int VoertuigenVoorLichtInt = static_cast<int> (VoertuigenVoorLicht.size());

                for (int i = VoertuigenVoorLichtInt - 2 ; i >= 0; i--) {
                    if (!VoertuigenVoorLicht[i]->getPrioriteit()) {
                        eerstVoertuigVoorLicht = VoertuigenVoorLicht[i];
                        break;
                    }
                }
            }

                // 3.1 ELSE IF verkeerslicht is rood
                if (isVoertuigInVertraagZone(eerstVoertuigVoorLicht, licht)) {
                    // 3.1.1 THEN IF het eerste voertuig voor het licht bevindt zich in de vertraagafstand
                    BerekenSnelheidNaVertraging(eerstVoertuigVoorLicht);
                    // 3.1.1.1 THEN pas de vertraagfactor toe op het voertuig
                } else if (isVoertuigInStopZone(eerstVoertuigVoorLicht, licht)) {
                    // 3.1.2 ELSE IF het eerste voertuig voor het licht bevindt zich in de eerste helft van de stopafstand
                    eerstVoertuigVoorLicht->UpdateVersnellingVoorStoppen(); // 3.1.1.1 THEN laat het voertuig stoppen
                } else if  (((eerstVoertuigVoorLicht->getPositie() > licht->getPositie() - STOP_AFSTAND / 2) == true) &&
                    ((eerstVoertuigVoorLicht->getPositie() < licht->getPositie()) == true)) {
                    eerstVoertuigVoorLicht->setSnelheid(0);
                    eerstVoertuigVoorLicht->setVersnelling(0);
                    }else {
                        BerekenSnelheidNaVersnelling(eerstVoertuigVoorLicht);
                    }

        }
    }
}

void simulation::generateSimulation() {
    simPr.generateSimulation(voertuigen,verkeerslichten,bushaltes,banen);
}

void simulation::updateSimulation() {
    simPr.updateSimulation();
}


