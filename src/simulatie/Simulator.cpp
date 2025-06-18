//
// Created by Abdellah on 4/28/2025.
//

#include "Simulator.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "../elementen/Constants.h"
#include "../elementen/Verkeerslicht.h"
#include "../elementen/Bushalte.h"
#include "../elementen/Baan.h"
#include "../elementen/Kruispunt.h"
#include "../elementen/Voertuiggenerator.h"

Simulator::Simulator(ErrorOutput& errorOutput)
        : _initCheck(this), errorOutput(&errorOutput) {
    // Constructor met ErrorOutput - aanbevolen
}

const vector<Baan*> Simulator::getBanen() const
{
    return banen;
}

void Simulator::addBaan(Baan* b)
{
    banen.push_back(b);
}

void Simulator::logError(const string& message, const string& context) {
    if (errorOutput != nullptr) {
        errorOutput->handleError(message, context);
    } else {
        // Fallback naar std::cerr als er geen ErrorOutput is
        std::cerr << "[ERROR] ";
        if (!context.empty()) {
            std::cerr << "[" << context << "] ";
        }
        std::cerr << message << std::endl;
    }
}

void Simulator::simulationRun()
{
    for (const auto b : banen)
    {
        b->sortVoertuigenByPosition();

        for (const auto bushalte : b->getBushaltes())
            bushalte->stopBus();

        for (const auto v : b->getVoertuigen())
        {
            const auto old_position = v->getPositie();
            v->rijd();

            if (v->opKruispunt(old_position))
                v->chooseKruispunt();
        }

        for (const auto vl : b->getVerkeerslichten())
            vl->updateVerkeerslicht();

        for (const auto g : b->getVoertuigeneratoren())
            g->generateVoertuig();

        for (const auto kruispunt : b->getKruispunten())
        {
            if (auto [hasLights, lichten] = kruispunt->verkeerslichtenStaanOpKruispunt(); hasLights)
            {
                Verkeerslicht* bestCandidate = *std::max_element(
                        lichten.begin(), lichten.end(),
                        [](Verkeerslicht* a, Verkeerslicht* b)
                        {
                            return a->getWaitingVehicles() < b->getWaitingVehicles();
                        });

                Verkeerslicht* current_green = kruispunt->getActiveVerkeerslicht();
                if (current_green == nullptr)
                {
                    current_green = bestCandidate;
                    kruispunt->setActiveVerkeerslicht(bestCandidate);

                    for (auto l : lichten)
                        l->setCyclus(bestCandidate->getCyclus());
                }

                kruispunt->increaseTimeSince(SIMULATIE_TIJD);

                const bool mag_veranderen = current_green->getTimeSince() > 10;
                const bool moet_veranderen = current_green->getTimeSince() >= 60;

                if (!mag_veranderen) continue;

                if (moet_veranderen)
                {
                    vector<Verkeerslicht*> copy(lichten.begin(), lichten.end());
                    while (copy.size() > 1 && bestCandidate == current_green)
                    {
                        copy.erase(remove(copy.begin(), copy.end(), current_green), copy.end());

                        bestCandidate = *std::max_element(
                                copy.begin(), copy.end(),
                                [](const Verkeerslicht* a, const Verkeerslicht* b)
                                {
                                    return a->getWaitingVehicles() < b->getWaitingVehicles();
                                });
                    }

                    kruispunt->setActiveVerkeerslicht(bestCandidate);
                }

                if (!moet_veranderen && bestCandidate != current_green && bestCandidate->getWaitingVehicles() >
                                                                          current_green->getWaitingVehicles())
                {
                    kruispunt->setActiveVerkeerslicht(bestCandidate);
                }
            }
        }
    }

    current_time += SIMULATIE_TIJD;
}

void Simulator::simulate(const int times)
{
    if (times <= 0) {
        logError("Aantal simulatie stappen moet groter zijn dan 0", "Simulator::simulate");
        return;
    }

    try {
        for (int i = 0; i < times; i++)
        {
            makeGraphicalImpression();
            simulationRun();
        }

        generateGraphicsFile();
    } catch (const std::exception& e) {
        if (errorOutput != nullptr) {
            errorOutput->handleException(e, "Simulator::simulate");
        } else {
            std::cerr << "Exception in simulate: " << e.what() << std::endl;
        }
    }
}

void Simulator::makeGraphicalImpression()
{
    string output_string = "Tijd: " + to_string(current_time) + "\n";

    for (const auto b : banen)
    {
        // ========== Fill the baan array with '=' ============
        string baan(b->getLengte(), '=');
        string verkeerslichten(b->getLengte(), ' ');
        string bushaltes(b->getLengte(), ' ');

        string second_line(b->getLengte(), ' ');
        // =========== Set the voertuigen in the baan ============
        for (const auto v : b->getVoertuigen())
        {
            const int p = round(v->getPositie());
            baan[p] = v->getType() == "Brandweerwagen" ? 'I' : v->getType()[0];
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
            if (p >= 0 && p < static_cast<int>(verkeerslichten.size())) {
                verkeerslichten[p] = '|';
                bushaltes[p] = 'B';
                second_line[p] = '|';
            } else {
                logError("Bushalte positie buiten baan grenzen: " + to_string(p), "Simulator::makeGraphicalImpression");
            }
        }

        for (const auto v : b->getVerkeerslichten())
        {
            const int p = round(v->getPositie());
            if (p >= 0 && p < static_cast<int>(verkeerslichten.size())) {
                verkeerslichten[p] = v->getState() == LightState::GREEN
                                     ? 'G'
                                     : v->getState() == LightState::RED
                                       ? 'R'
                                       : 'O';

                second_line[p] = v->getState() == LightState::GREEN ? 'G' : v->getState() == LightState::RED ? 'R' : 'O';
            } else {
                logError("Verkeerslicht positie buiten baan grenzen: " + to_string(p), "Simulator::makeGraphicalImpression");
            }
        }

        // ============ Print the baan and the voertuigen =============
        output_string.append(baan_text + baan + "\n");
        output_string.append(verkeerslichten_text + verkeerslichten + "\n");
        output_string.append(bushaltes_text + bushaltes + "\n\n");
    }
    graphical_impression += output_string + "\n\n";
}

void Simulator::generateGraphicsFile() const
{
    try {
        // Make sure "output" folder exists
        if (!filesystem::exists("../output"))
        {
            filesystem::create_directory("../output");
        }

        // Create an output directory first (manually or automatically)
        ofstream file("../output/simulation_output.txt"); // Write to file inside 'output' folder

        if (file.is_open())
        {
            file << graphical_impression; // Write the string into the file
            file.close(); // Always close the file
        }
        else
        {
            if (errorOutput != nullptr) {
                errorOutput->logError("Failed to open file: ../output/simulation_output.txt");
            }
        }
    } catch (const std::exception& e) {
        if (errorOutput != nullptr) {
            errorOutput->handleException(e, "Simulator::generateGraphicsFile");
        }
    }
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

void Simulator::printStatus(Voertuig const* voertuig)
{
    cout << "Voertuig " << voertuig->getId() << "\n"
         << "-> baan: " << voertuig->getBaan()->getNaam() << "\n"
         << "-> positie: " << voertuig->getPositie() << "\n"
         << "-> snelheid: " << voertuig->getSnelheid() << endl;
}

void Simulator::geldigeTypen(const string& type)
{
    isConsistent = type == "auto" || type == "bus" || type == "brandwagen" || type == "politiecombi" || type ==
                                                                                                        "ziekenwagen";

    if (!isConsistent) {
        logError("Ongeldig voertuigtype: " + type, "Simulator::geldigeTypen");
    }
}