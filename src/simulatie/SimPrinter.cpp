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
    for (auto b: banen) {
        bool firstTime = true;
        string s = "BN|LT\n >verkeerslichten\t\t|VL\n >bushaltes\t\t\t\t|BH\n";
        int bsT = 0;
        string BN = b->getNaam();
        if (BN.size()<28){
            int difference = 28 - BN.size();
            bsT = difference/4;
            BN = BN+string(bsT,'\t');
        }
        string LT((b->getLengte() / simulatieSchaal), '=');
        string VL((b->getLengte() / simulatieSchaal), ' ');
        string BH((b->getLengte() / simulatieSchaal), ' ');
        int index1 = 0;
        int index2 = 0;
        if (firstTime) {
            index1 = BN.size() + 1;
            index2 = 22+ LT.size() + BN.size();
            vtXvlIndex[b] = {index1, index2};
            firstTime = false;
        }

        for (auto v: voertuigen) {
            if (v->getBaan()->getNaam() == b->getNaam()) {
                int voertuigpositie = v->getPositie() / simulatieSchaal;
                if (v->getType() == VoertuigType::AUTO) {
                    LT[voertuigpositie] = 'A';
                    voertuigenOpBaanSIM[b][v->getPositie()] = {v, 'A'};
                } else if (v->getType() == VoertuigType::BUS) {
                    LT[voertuigpositie] = 'B';
                    voertuigenOpBaanSIM[b][v->getPositie()]= {v, 'B'};
                } else if (v->getType() == VoertuigType::BRANDWEERWAGEN) {
                    LT[voertuigpositie] = 'I';
                    voertuigenOpBaanSIM[b][v->getPositie()] = {v, 'I'};
                } else if (v->getType() == VoertuigType::ZIEKENWAGEN) {
                    LT[voertuigpositie] = 'Z';
                    voertuigenOpBaanSIM[b][v->getPositie()] = {v, 'Z'};
                }else if (v->getType() == VoertuigType::POLITIECOMBI) {
                    LT[voertuigpositie] = 'P';
                    voertuigenOpBaanSIM[b][v->getPositie()] = {v, 'P'};
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
}

void SimPrinter::addNewGeneratedVoertuigen(Baan*b, char c, Voertuig *v) {
    voertuigenOpBaanSIM[b][v->getPositie()]= {v,c};
}

void SimPrinter::updateSimulation() {

    //we behandelen elk baan met zijn voertuigen en verkeerslichten
    int counter = 0;
    for (auto b: voertuigenOpBaanSIM) {
        for (auto v: b.second) {
                int Vindex = vtXvlIndex[b.first].first + v.second.first->getPositie() / simulatieSchaal;
                ////////////////////ik was hier geindigd
                //we kijken of de voertuig op de baan is
                if (v.second.first->getPositie() <= b.first->getLengte()) {
                    //we kijken of de nieuwe index en de oude index het zelfde zijn
                    if (Gsim[counter][Vindex] == '=') {
                        //iteratie skippen
                        continue;
                    }
                        //als de nieuwe index groter is
                    else if (Vindex > v.first) {
                        //dan gaan we in de simulatie de characters updaten
                        Gsim[counter][v.first] = '=';
                        Gsim[counter][Vindex] = v.second.second;

                        //we moeten dan ook de informatie in de mappen aanpassen aan de nieuwe gegevens
                        Voertuig *v2 = v.second.first;
                        char voertuigType = v.second.second;
                        voertuigenOpBaanSIM[b.first].erase(v.first);
                        voertuigenOpBaanSIM[b.first][Vindex] = {v2, voertuigType};

                    }
                    //als de voertuig buiten de baan ligt dan moeten we het uit de baan map verwijderen en uit de simulatie string ook.
                } else if (v.second.first->getPositie() > b.first->getLengte()) {
                    Gsim[counter][v.first] = '=';
                    voertuigenOpBaanSIM[b.first].erase(v.first);
                }
            counter++;
        }
    }

    counter = 0;
    for (auto b: verkeerslichtenOpBaanSIM) {
        for (auto vl: b.second) {
            int vlIndex = vtXvlIndex[b.first].second + vl.first;
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
}

void SimPrinter::generateTXT() {
    ofstream myFile("../src/output/simulatie.txt");

    myFile << "Simulatie 0:" << endl;
    for (auto s: Gsim){
        myFile<<s<<endl;
    }
    myFile << string(24,'-') << endl;
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
        myFile << string(24,'-') << endl;

        // Close the file
        myFile.close();
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