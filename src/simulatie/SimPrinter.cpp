//
// Created by s0246031@ad.ua.ac.be on 3/17/25.
//

#include "SimPrinter.h"
#include <iostream>
#include <regex>
#include <fstream>

using namespace std;


void SimPrinter::printStatus(Voertuig const* voertuig, double status) {
    cout << "Tijd: " << status << endl;

    cout << "Voertuig " << voertuig->getId() << "\n"
        << "-> baan: " << voertuig->getBaan()->getNaam()  << "\n"
        << "-> positie: " << voertuig->getPositie() << "\n"
        << "-> snelheid: " << voertuig->getSnelheid() << "\n" <<endl;
}

void SimPrinter::generateSimulation(vector<Voertuig *> &voertuigen,
                                    vector<Verkeerslicht *> &verkeerslichten, vector<Bushalte *> &bushaltes, vector<Baan *> &banen) {
    vector<string> ss;
    bool firstTime = true;
    for (auto b: banen) {
        string s = "BN\t\t\t|LT\n >verkeerslichten\t\t|VL\n >bushaltes\t\t\t\t|BH\n";

        string BN = b->getNaam();
        string LT((b->getLengte() / simulatieSchaal), '=');
        string VL((b->getLengte() / simulatieSchaal), ' ');
        string BH((b->getLengte() / simulatieSchaal), ' ');
        int index1 = 0;
        int index2 = 0;
        if (firstTime) {
            index1 = BN.size() + 4;
            index2 = 25 + LT.size() + BN.size();
            vtXvlIndex = {index1, index2};
            firstTime = false;
        }

        for (auto v: voertuigen) {
            if (v->getBaan()->getNaam() == b->getNaam()) {
                int voertuigpositie = v->getPositie() / simulatieSchaal;
                if (v->getType() == VoertuigType::AUTO) {
                    LT[voertuigpositie] = 'A';
                    voertuigenOpBaanSIM[b][voertuigpositie+index1] = v;
                } else if (v->getType() == VoertuigType::BUS) {
                    LT[voertuigpositie] = 'B';
                    voertuigenOpBaanSIM[b][voertuigpositie+index1] = v;
                } else if (v->getType() == VoertuigType::BRANDWEERWAGEN) {
                    LT[voertuigpositie] = 'I';
                    voertuigenOpBaanSIM[b][voertuigpositie+index1] = v;
                } else if (v->getType() == VoertuigType::ZIEKENWAGEN) {
                    LT[voertuigpositie] = 'Z';
                    voertuigenOpBaanSIM[b][voertuigpositie+index1] = v;
                } else if (v->getType() == VoertuigType::POLITIECOMBI) {
                    LT[voertuigpositie] = 'P';
                    voertuigenOpBaanSIM[b][voertuigpositie+index1] = v;
                }
            }
        }
        for (auto vl: verkeerslichten) {
            if (vl->getBaan()->getNaam() == b->getNaam()) {
                int vlPositie = vl->getPositie() / simulatieSchaal;
                if (vl->isRood()) {
                    VL[vlPositie] = 'R';
                    verkeerslichtenOpBaanSIM[b][vlPositie] = vl;
                } else if (vl->isGroen()) {
                    VL[vlPositie] = 'G';
                    verkeerslichtenOpBaanSIM[b][vlPositie] = vl;
                }
            }
        }
        for (auto bh: bushaltes) {
            if (bh->getBaan()->getNaam() == b->getNaam()) {
                int bhPositie = bh->getPositie() / simulatieSchaal;
                VL[bhPositie] = '|';
                BH[bhPositie] = 'B';
            }
        }
        s = regex_replace(s, regex("BN"), BN);
        s = regex_replace(s, regex("LT"), LT);
        s = regex_replace(s, regex("VL"), VL);
        s = regex_replace(s, regex("BH"), BH);

        ss.push_back(s);
    }
    Gsim = ss;
    generateTXT();
    generateHTML();
}

void SimPrinter::updateSimulation() {
    //we behandelen elk baan met zijn voertuigen en verkeerslichten
    for (auto b: voertuigenOpBaanSIM) {
        int counter = 0;
        for (auto v: b.second) {
            //we berekenen de "nieuwe" index
            int Vindex = vtXvlIndex.first + v.second->getPositie() / simulatieSchaal;

            //we kijken of de voertuig op de baan is
            if (v.second->getPositie() <= b.first->getLengte()) {
                //we kijken of de nieuwe index en de oude index het zelfde zijn
                if (Vindex == v.first) {
                    //iteratie skippen
                    continue;
                }
                    //als de nieuwe index groter is
                else if (Vindex > v.first) {
                    //dan gaan we in de simulatie de characters updaten
                    char c = Gsim[counter][v.first];
                    Gsim[counter][v.first] = '=';
                    Gsim[counter][Vindex] = c;

                    //we moeten dan ook de informatie in de mappen aanpassen aan de nieuwe gegevens
                    Voertuig *v2 = v.second;
                    voertuigenOpBaanSIM[b.first].erase(v.first);
                    voertuigenOpBaanSIM[b.first][Vindex] = v2;

                }
                //als de voertuig buiten de baan ligt dan moeten we het uit de baan map verwijderen en uit de simulatie string ook.
            } else if (v.second->getPositie() > b.first->getLengte()) {
                Gsim[counter][v.first] = '=';
                voertuigenOpBaanSIM[b.first].erase(v.first);
            }
        }
        counter++;
    }

    for (auto b: verkeerslichtenOpBaanSIM) {
        int counter = 0;
        for (auto vl: b.second) {
            int vlIndex = vtXvlIndex.second + vl.first;
            if (vl.second->isRood()){
                if(Gsim[counter][vlIndex] == 'R'){
                    continue;
                }
                else if (Gsim[counter][vlIndex] == 'G'){
                    Gsim[counter][vlIndex] = 'R';
                }
            }
            else if(vl.second->isGroen()){
                if(Gsim[counter][vlIndex] == 'G'){
                    continue;
                }
                else if (Gsim[counter][vlIndex] == 'R'){
                    Gsim[counter][vlIndex] = 'G';
                }
            }
        }
        counter++;
    }
    simCounter++;
    updateTXT();
    updateHTML();
}

void SimPrinter::printSimulation() {
    for (auto &s: Gsim){
        cout<<s<<endl;
    }
}

void SimPrinter::generateTXT() {
    ofstream myFile("../src/output/simulatie.txt");

    myFile << "Simulatie 0:" << endl;
    for (auto s: Gsim){
        myFile<<s<<endl;
    }
    myFile << string(vtXvlIndex.first,'-') << endl;
    myFile.close();
}

void SimPrinter::updateTXT() {

    std::ofstream myFile("../src/output/simulatie.txt", std::ios::app);

    // Check if the file is open
    if (myFile.is_open()) {
        myFile << "Simulatie " << to_string(simCounter) <<':'<<endl;
        for (auto s: Gsim){
            myFile << s << endl;
        }
        myFile << string(vtXvlIndex.first,'-') << endl;

        // Close the file
        myFile.close();
    }
}

void SimPrinter::generateHTML() {
    ofstream myFile("../src/output/simulatie.html");

    // Write HTML header
    myFile << "<!DOCTYPE html>" << endl;
    myFile << "<html lang=\"en\">" << endl;
    myFile << "<head>" << endl;
    myFile << "    <meta charset=\"UTF-8\">" << endl;
    myFile << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">" << endl;
    myFile << "    <title>Simulation Results</title>" << endl;
    myFile << "    <style>" << endl;
    myFile << "        body { font-family: monospace; padding: 20px; }" << endl;
    myFile << "        .simulation { margin-bottom: 20px; }" << endl;
    myFile << "        .simulation-title { font-weight: bold; margin-bottom: 5px; }" << endl;
    myFile << "        pre { margin: 0; }" << endl;
    myFile << "        .divider { color: #666; margin: 10px 0; }" << endl;
    myFile << "    </style>" << endl;
    myFile << "</head>" << endl;
    myFile << "<body>" << endl;

    // Write first simulation
    myFile << "<div class=\"simulation\">" << endl;
    myFile << "    <div class=\"simulation-title\">Simulatie 0:</div>" << endl;
    myFile << "    <pre>";
    for (auto s: Gsim){
        // Check if this is the line with bushaltes
        if (s.find(">bushaltes") != string::npos) {
            // Find the position of the last pipe character
            size_t posBar = s.rfind("|");

            if (posBar != string::npos && posBar > 0) {
                // Find the last tab before the pipe
                size_t posLastTab = s.rfind("\t", posBar);

                if (posLastTab != string::npos) {
                    // Remove the last tab
                    string fixedLine = s.substr(0, posLastTab) + s.substr(posLastTab + 1);
                    myFile << fixedLine << "\n";
                } else {
                    myFile << s << "\n";
                }
            } else {
                myFile << s << "\n";
            }
        } else {
            myFile << s << "\n";
        }
    }
    myFile << "    </pre>" << endl;
    myFile << "</div>" << endl;

    // Add divider line using the street length
    myFile << "<div class=\"divider\">" << string(vtXvlIndex.first, '-') << "</div>" << endl;

    // Write HTML footer
    myFile << "</body>" << endl;
    myFile << "</html>" << endl;

    myFile.close();
}

void SimPrinter::updateHTML() {
    // Read the existing file content
    ifstream readFile("../src/output/simulatie.html");
    string content((istreambuf_iterator<char>(readFile)), istreambuf_iterator<char>());
    readFile.close();

    // Find the position to insert new content (before the closing </body> tag)
    size_t insertPos = content.find("</body>");
    if (insertPos != string::npos) {
        // Create new simulation content
        string newSimulation = "<div class=\"simulation\">\n";
        newSimulation += "    <div class=\"simulation-title\">Simulatie " + to_string(simCounter) + ":</div>\n";
        newSimulation += "    <pre>";
        for (auto s: Gsim) {
            // Check if this is the line with bushaltes
            if (s.find(">bushaltes") != string::npos) {
                // Find the position of the last pipe character
                size_t posBar = s.rfind("|");

                if (posBar != string::npos && posBar > 0) {
                    // Find the last tab before the pipe
                    size_t posLastTab = s.rfind("\t", posBar);

                    if (posLastTab != string::npos) {
                        // Remove the last tab
                        string fixedLine = s.substr(0, posLastTab) + s.substr(posLastTab + 1);
                        newSimulation += fixedLine + "\n";
                    } else {
                        newSimulation += s + "\n";
                    }
                } else {
                    newSimulation += s + "\n";
                }
            } else {
                newSimulation += s + "\n";
            }
        }
        newSimulation += "    </pre>\n";
        newSimulation += "</div>\n";

        // Add divider line using the street length
        newSimulation += "<div class=\"divider\">" + string(vtXvlIndex.first, '-') + "</div>\n";

        // Insert the new content
        content.insert(insertPos, newSimulation);

        // Write the updated content back to the file
        ofstream writeFile("../src/output/simulatie.html");
        writeFile << content;
        writeFile.close();
    }
}



// De 2de printstatus geeft meer kenmerken van d auto kan handig zijn voor als je iets will checken
// !!!! DUS NIET VERWIJDEREN !!!

/*
void SimPrinter::printStatus(Voertuig const* voertuig, double status) {
    cout << "Tijd: " << status << endl;

    // Bepaal het type voertuig als string
    string typeStr;
    switch(voertuig->getType()) {
        case VoertuigType::AUTO: typeStr = "Auto"; break;
        case VoertuigType::BUS: typeStr = "Bus"; break;
        case VoertuigType::BRANDWEERWAGEN: typeStr = "Brandweerwagen"; break;
        case VoertuigType::ZIEKENWAGEN: typeStr = "Ziekenwagen"; break;
        case VoertuigType::POLITIECOMBI: typeStr = "Politiecombi"; break;
        default: typeStr = "Onbekend"; break;
    }

    cout << "Voertuig " << voertuig->getId() << " (" << typeStr << ")\n"
         << "-> baan: " << voertuig->getBaan()->getNaam() << "\n"  // Gebruik getNaam() voor de baan
         << "-> positie: " << voertuig->getPositie() << "\n"
         << "-> snelheid: " << voertuig->getSnelheid() << "\n"
         << "-> lengte: " << voertuig->getLength() << "\n"<<endl;
}
 */