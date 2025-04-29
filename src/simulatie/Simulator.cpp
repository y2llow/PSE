//
// Created by eraya on 13/03/2025.
//

#include "Simulator.h"
#include "../elementen/Constants.h"
#include <iostream>
#include <map>
#include <string>
#include "../../src/TinyXML/tinyxml.h"
#include <algorithm>
#include <cmath>
#include <regex>

#include "SimPrinter.h"
#include <filesystem>
#include <fstream>


/*
// Simulator::Simulator() {
//     simulationTime = 0;
//     simulationincreasedTime = 0;
//     lastGeneretedVoertuigTime = 0;
//     voertuigLastId = 1;
// }
//
// int Simulator::getVoertuigLastId() const {
//     return voertuigLastId;
// }
//
// void Simulator::increaseVoertuigLastId() {
//     voertuigLastId++;
// }

// SimPrinter * simulation::getSimPrinter() const {
//     return simPrinter;
// }
//
// void simulation::setSimPrinter(SimPrinter * const sim_printer) {
//     simPrinter = sim_printer;
// }



vector<Voertuig *> Simulator::getVoertuigen() const {
    // Don't return the original vector, return a new copy with the same values so that the original one cannot get edited
    vector<Voertuig*> vv;
    for (auto* b : voertuigen) {
        vv.push_back(b);
    }
    return vv;
}

vector<Verkeerslicht *> Simulator::getVerkeerslichten() const {
    // Don't return the original vector, return a new copy with the same values so that the original one cannot get edited
    vector<Verkeerslicht*> vv;
    for (auto* b : verkeerslichten) {
        vv.push_back(b);
    }
    return vv;
}

vector<Bushalte *> Simulator::getBushaltes() const {
    // Don't return the original vector, return a new copy with the same values so that the original one cannot get edited
    vector<Bushalte*> bb;
    for (auto* b : bushaltes) {
        bb.push_back(b);
    }
    return bb;
}

vector<Voertuiggenerator *> Simulator::getVoertuiggeneratoren() const {
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


void Simulator::ToString() {
    for (Voertuig *voertuig: voertuigen) {
        SimPrinter::printStatus(voertuig, getincSimulationTime());

    }
}

double Simulator::getSimulationTime() const {
    return simulationTime;
}

double Simulator::getincSimulationTime() const {
    return simulationincreasedTime;
}

double Simulator::incSimulationTime() {
    simulationincreasedTime = simulationincreasedTime + SIMULATIE_TIJD;
    return simulationincreasedTime;
}

double Simulator::UpdateSimulationTime() const {
    return simulationTime + SIMULATIE_TIJD;
}

void Simulator::sortVoertuigenByPosition() {
    sort(voertuigen.begin(), voertuigen.end(), [](const Voertuig *a, const Voertuig *b) {
        return a->getPositie() < b->getPositie();
    });
}

void Simulator::sortVerkeersLichtByPosition() {
    sort(verkeerslichten.begin(), verkeerslichten.end(), [](const Verkeerslicht *a, const Verkeerslicht *b) {
        return a->getPositie() < b->getPositie();
    });
}

void Simulator::berekenPositie(Voertuig *v) const {
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

void Simulator::BerekenVersnelling(Voertuig *v, int counter) const {
    double delta = 0;

    size_t vsize = voertuigen.size();
    int sizeVoertuigen = vsize;

        if (sizeVoertuigen > counter + 1) {
            double volgafstand = voertuigen[counter + 1]->getPositie() - v->getPositie() - v->getLengte();
            double snelheidVerschil = v->getSnelheid() - voertuigen[counter + 1]->getSnelheid();

            double newsnelheid = v->getSnelheid() * snelheidVerschil;
            double newversnelling = 2 * sqrt(v->getMaximaleVersnelling() * v->getMaximaleRemfactor());

            double calculate = v->getSnelheid() + (newsnelheid / newversnelling);

            double maxNummer = max(0.0, calculate);
            delta = (v->getMinimaleVolgafstand() + maxNummer) / volgafstand;

            double newVersnelling = v->getMaximaleVersnelling() * (1 - pow((v->getSnelheid() / v->getKvmax()), 4) - pow(delta, 2));
            v->setVersnelling(newVersnelling);
        } else {
            double newVersnelling = v->getVersnelling() * (1 - pow((v->getSnelheid() / v->getKvmax()), 4) -
                                                       pow(delta, 2));
            v->setVersnelling(newVersnelling);
        }
    }


void Simulator::updateVoertuig(Voertuig *v, int counter) const {
    berekenPositie(v);
    BerekenVersnelling(v, counter);
}



bool Simulator::isVoertuigOpBaan(const Voertuig *v) {
    // Baan *baan = getBaanByName(v->getBaan()); TODO:// de baan van de voertuig

    Baan* baan = v->getBaan();

    return v->getPositie() <= baan->getLengte();
}

void Simulator::simulationRun() {
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

bool Simulator::isVoertuigInVertraagZone(Voertuig *v, Verkeerslicht *l) {
    // Bepaal of het voertuig binnen de vertragingsafstand is
    bool isInVertraagZone = abs(v->getPositie() - l->getPositie()) <= VERTRAAG_AFSTAND;

    // Bepaal of het voertuig buiten de stopzone is
    bool isOutsideStopZone = v->getPositie() < l->getPositie() - STOP_AFSTAND;

    // Het voertuig is in de vertragingszone als het binnen de vertragingsafstand is en buiten de stopzone
    return isInVertraagZone && isOutsideStopZone;
}

bool Simulator::isVoertuigInStopZone(Voertuig *v, Verkeerslicht *l) {
    bool isInStopZone1 = v->getPositie() < l->getPositie() - STOP_AFSTAND / 2;
    bool isInStopZone2 = v->getPositie() >= l->getPositie() - STOP_AFSTAND;

    return isInStopZone1 == true && true == isInStopZone2;
}

void Simulator::BerekenSnelheidNaVertraging(Voertuig *v) {
    v->setKvmax(v->getMaximaleSnelheid() * VERTRAAG_FACTOR);
}

void Simulator::BerekenSnelheidNaVersnelling(Voertuig *v) {
    v->setKvmax(v->getMaximaleSnelheid());
}



vector<Verkeerslicht *> Simulator::verkeerslichtenOpBaan(Verkeerslicht *licht) {
    vector<Verkeerslicht *> verkeersLichtenOpDezelfdeBaan;
    for (Verkeerslicht *v: verkeerslichten) {
        if (licht->getBaan() == v->getBaan()) {
            verkeersLichtenOpDezelfdeBaan.push_back(v);
        }
    }
    return verkeersLichtenOpDezelfdeBaan;
}

void Simulator::addVoertuig(Voertuig *v) {
    voertuigen.push_back(v);
}

void Simulator::addVerkeerslicht(Verkeerslicht *v) {
    verkeerslichten.push_back(v);
}

void Simulator::addBushalte(Bushalte *b) {
    bushaltes.push_back(b);
}


void Simulator::addVoertuiggenerator(Voertuiggenerator *v) {
    voertuiggeneratoren.push_back(v);
}

void Simulator::voertuigenGenereren() {
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
                // // Haal het type op uit de generator
                // VoertuigType type = v->getType();

                // Maak een nieuw voertuig met het juiste type
                Voertuig* generated_v = Voertuig::createVoertuig(v->getType());

                // Zet de nodige attributen
                generated_v->setId(voertuigLastId);
                generated_v->setSnelheid(generated_v->getMaximaleSnelheid()); // Gebruik de maximale snelheid van dit type
                generated_v->setKvmax(generated_v->getMaximaleSnelheid());    // Gebruik de maximale snelheid van dit type

                voertuigLastId++;

                voertuigen.push_back(generated_v);
                sortVoertuigenByPosition();

                lastGeneretedVoertuigTime = simulationTime;
            }
        }
    }
}

void Simulator::updateVoertuigAanVerkeerslichtSituatie(Verkeerslicht *licht, int verkeerslichtCounter) {
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
            if (eerstVoertuigVoorLicht->isPrioriteitsVoertuig()) {
                BerekenSnelheidNaVersnelling(eerstVoertuigVoorLicht);
                int VoertuigenVoorLichtInt = static_cast<int> (VoertuigenVoorLicht.size());

                for (int i = VoertuigenVoorLichtInt - 2 ; i >= 0; i--) {
                    if (!VoertuigenVoorLicht[i]->isPrioriteitsVoertuig()) {
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
                    eerstVoertuigVoorLicht->updateVersnellingVoorStoppen(); // 3.1.1.1 THEN laat het voertuig stoppen
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


*/
void Simulator::addBaan(Baan *b) {
    banen.push_back(b);
}


void Simulator::makeGraphicalImpression()
{
    for (const auto b : banen)
    {
        // ========== Fill the baan array with '=' ============
        string baan(b->getLengte(), '=');
        string verkeerslichten(b->getLengte(), ' ');
        string bushaltes(b->getLengte(), ' ');

        // =========== Set the voertuigen in the baan ============
        for (const auto v : b->getVoertuigen())
        {
            const int p = round(v->getPositie());
            baan[p] = v->getType()[0];
        }

        // =========== Get the right length of the texts in the first column ===========
        string baan_text = b->getNaam();
        string verkeerslichten_text = " > verkeerslichten   ";
        string bushaltes_text = " > bushaltes         ";


        if (b->getNaam().size() > 21)
        {
            for (size_t i = 0, n = b->getNaam().size(); i < n - 20; i++)
            {
                verkeerslichten_text.push_back(' ');
                bushaltes_text.push_back(' ');
            }

            baan_text.push_back(' ');
        }
        else
        {
            for (size_t i = 0, n = b->getNaam().size(); i < 21 - n; i++)
                baan_text.push_back(' ');
        }

        verkeerslichten_text.append("| ");
        bushaltes_text.append("| ");
        baan_text.append("| ");


        // ============ Fill the baan with ' ' so that we can print the verkeerslichten =============

        for (const auto bushalte : b->getBushaltes())
        {
            const int p = round(bushalte->getPositie());
            verkeerslichten[p] = '|';
            bushaltes[p] = 'B';
        }

        for (const auto v : b->getVerkeerslichten())
        {
            const int p = round(v->getPositie());
            verkeerslichten[p] = v->isGroen() ? 'G' : 'R';
        }

        // ============ Print the baan and the voertuigen =============
        // ============ Then print the verkeerslichten ==============
        // ============ Then print the bushaltes ==============
        string output_string = "Tijd: " + to_string(current_time) + "\n";
        output_string.append(baan_text + baan + "\n");
        output_string.append(verkeerslichten_text + verkeerslichten + "\n");
        output_string.append(bushaltes_text + bushaltes + "\n\n");

        graphical_impression += output_string;
    }
}

void Simulator::generateGraphicsFile()
{
    // Make sure "output" folder exists
    if (!filesystem::exists("../output"))
    {
        filesystem::create_directory("../output");
    }
    // Create an output directory first (manually or automatically)

    if (ofstream file("../output/simulation_output.txt"); file.is_open())
    {
        file << graphical_impression; // Write the string into the file
        file.close(); // Always close the file
    }
    else
    {
        std::cerr << "Failed to open file!" << std::endl;
    }
}

void Simulator::simulate(const int times)
{
    for (int i = 0; i < times; i++)
    {
        makeGraphicalImpression();
        // print();

        simulationRun();
    }

    generateGraphicsFile();
}

void Simulator::geldigeTypen(const string& type)
{
    is_consistent = type == "auto" || type == "bus" || type == "brandwagen" || type == "politiecombi" || type ==
        "ziekenwagen";
}

const vector<Baan *> Simulator::getBanen() const {
    // Don't return the original vector, return a new copy with the same values so that the original one cannot get edited
    vector<Baan*> bb;
    for (auto* b : banen) {
        bb.push_back(b);
    }
    return bb;
}

void Simulator::simulationRun()
{
    for (const auto b : banen)
    {
        b->sortVoertuigenByPosition();

        for (const auto bushalte : b->getBushaltes())
            bushalte->stopBus();

        for (const auto v : b->getVoertuigen())
            v->rijd();

        for (const auto vl : b->getVerkeerslichten())
            vl->updateVerkeerslicht();

        for (const auto g : b->getVoertuigeneratoren())
            g->generateVoertuig();
    }


    current_time += SIMULATIE_TIJD;
}

void Simulator::print()
{
    cout << "Tijd: " << current_time << endl;
    for (const auto b : banen)
    {
        for (const auto v : b->getVoertuigen())
            printStatus(v);
    }
    cout << endl;
}

void Simulator::printStatus(Voertuig const* voertuig) const
{
    cout << "Voertuig " << voertuig->getId() << "\n"
        << "-> baan: " << voertuig->getBaan()->getNaam() << "\n"
        << "-> positie: " << voertuig->getPositie() << "\n"
        << "-> snelheid: " << voertuig->getSnelheid() << endl;
}
