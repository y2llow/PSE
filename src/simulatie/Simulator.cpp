//
// Created by eraya on 13/03/2025.
//

#include "Simulator.h"


#include "../../src/elementen/Verkeerslicht.h"
#include "../../src/elementen/Bushalte.h"
#include "../../src/elementen/Voertuiggenerator.h"
#include "../elementen/Constants.h"
#include <iostream>
#include <map>
#include <string>
#include "../../src/TinyXML/tinyxml.h"
#include <algorithm>
#include <cmath>
#include <regex>

#include <filesystem>
#include <fstream>


void Simulator::addBaan(Baan* b)
{
    banen.push_back(b);
}

void Simulator::makeGraphicalImpression()
{
    for (const auto b : banen)
    {
        string output_string;

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
            verkeerslichten[p] = v->isGroen() ? 'G' : 'R';
            second_line[p] = v->isGroen() ? 'G' : 'R';
        }

        // ============ Print the baan and the voertuigen =============
        // ============ Then print the verkeerslichten ==============
        // ============ Then print the bushaltes ==============
        if (b == banen.front()) output_string = "Tijd: " + to_string(current_time) + "\n";
        output_string.append(baan_text + baan + "\n");
        output_string.append(verkeerslichten_text + verkeerslichten + "\n");
        output_string.append(bushaltes_text + bushaltes + "\n\n");

        graphical_impression += output_string;
        banen_3d_content[b->getNaam()] += baan + second_line + '\n';
    }
}

void Simulator::generateGraphicsFile() const
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

    for (const auto b : banen)
        generate3dfile(b->getNaam());
}


const vector<Baan*> Simulator::getBanen() const
{
    // Don't return the original vector, return a new copy with the same values so that the original one cannot get edited
    vector<Baan*> bb;
    for (auto* b : banen)
    {
        bb.push_back(b);
    }
    return bb;
}

void Simulator::simulationRun()
{
    for (const auto b : banen)
    {
        for (const auto g : b->getVoertuigeneratoren())
            g->generateVoertuig();

        b->sortVoertuigenByPosition();

        for (const auto vl : b->getVerkeerslichten())
            vl->updateVerkeerslicht();

        for (const auto bushalte : b->getBushaltes())
            bushalte->stopBus();

        for (const auto v : b->getVoertuigen()) {
            double positie = v->getPositie();
            v->rijd();
            double newpositie = v->getPositie();


            if (!b->getKruispunten().empty()) {
                // =========Voor meerderen banen aan een kruispunten=========
                v->checkForKruispunt(positie,newpositie);

            }
        }
    }


    current_time += SIMULATIE_TIJD;
}

void Simulator::generate3dfile(const string& baan)
{
    // Make sure "output" folder exists
    if (!filesystem::exists("../output"))
    {
        filesystem::create_directory("../output");
    }
    // Create an output directory first (manually or automatically)
    ofstream file("../output/baan_" + baan + ".txt"); // Write to file inside 'output' folder

    if (file.is_open())
    {
        file << banen_3d_content[baan]; // Write the string into the file
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
    cout << voertuig->getType() << ": " << voertuig->getId() << "\n"
        << "-> baan: " << voertuig->getBaan()->getNaam() << "\n"
        << "-> positie: " << voertuig->getPositie() << "\n"
        << "-> snelheid: " << voertuig->getSnelheid() << endl;
}
