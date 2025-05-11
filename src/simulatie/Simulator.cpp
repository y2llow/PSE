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


const vector<Baan*> Simulator::getBanen() const
{
    return banen;
}

void Simulator::addBaan(Baan* b)
{
    banen.push_back(b);
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
                    // cout << "+===============================================\n\n\n";
                }

                if (!moet_veranderen && bestCandidate != current_green && bestCandidate->getWaitingVehicles() >
                    current_green->getWaitingVehicles())
                {
                    // cout << current_time << endl;
                    // if (bestCandidate->getBaan()->getNaam() == "Groeneborgerlaan")
                    //     cout << "It has " << bestCandidate->getWaitingVehicles() << endl;
                    kruispunt->setActiveVerkeerslicht(bestCandidate);
                }


                // for (const auto l : lichten)
                // {
                //     if (l != kruispunt->getActiveVerkeerslicht() && l->getState() != LightState::ORANGE)
                //         l->setState(LightState::RED);
                // }
            }
        }
    }


    current_time += SIMULATIE_TIJD;
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
            verkeerslichten[p] = '|';
            bushaltes[p] = 'B';

            second_line[p] = '|';
        }

        for (const auto v : b->getVerkeerslichten())
        {
            const int p = round(v->getPositie());
            verkeerslichten[p] = v->getState() == LightState::GREEN
                                     ? 'G'
                                     : v->getState() == LightState::RED
                                     ? 'R'
                                     : 'O';

            second_line[p] = v->getState() == LightState::GREEN ? 'G' : v->getState() == LightState::RED ? 'R' : 'O';
        }

        // ============ Print the baan and the voertuigen =============
        // ============ Then print the verkeerslichten ==============
        // ============ Then print the bushaltes ==============
        output_string.append(baan_text + baan + "\n");
        output_string.append(verkeerslichten_text + verkeerslichten + "\n");
        output_string.append(bushaltes_text + bushaltes + "\n\n");
    }
    graphical_impression += output_string + "\n\n";
}

void Simulator::generateGraphicsFile() const
{
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
        std::cerr << "Failed to open file!" << std::endl;
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
}
