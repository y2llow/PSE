//
// Created by eraya on 13/03/2025.
//

#include "simulation.h"
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

    int voertuigLastId = 0; // TODO pas dit aan zodat we dit ergens anders kunnen opslagen
    for (TiXmlElement *elem = root->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement()) {
        string elementType = elem->Value();

        if (elementType == "BAAN") {
            Baan* baan = new Baan();
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
            if (geldig) {banen.push_back(baan);} // Add to vector

        } else if (elementType == "VOERTUIG") {
            Voertuig* voertuig = new Voertuig();
            bool geldig = true;

            voertuigLastId ++;
            voertuig->setId(voertuigLastId);

            //we geven vMax de waarde van Vmax
            voertuig->setKvmax(voertuig->getGVmax());

            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                string propertyName = subElem->Value();

                if (propertyName == "baan") {
                    if (!subElem->GetText()) {
                        cerr << "Er is een voertuig zonder baan!" << endl;
                        geldig = false;
                        break;
                    }

                    Baan voertuigbaan;
                    for (Baan* baan : banen) {
                        if (baan->getNaam() == subElem->GetText()) {
                            voertuigbaan.setNaam(baan->getNaam());
                            voertuigbaan.setLengte(baan->getLengte());
                        }
                    }
                    voertuig->setBaan(voertuigbaan);

                } else if (propertyName == "snelheid") {
                    string snelheidstring = subElem->GetText();
                    double snelheid = std::stod(snelheidstring);
                    voertuig->setSnelheid(snelheid);

                } else if (propertyName == "positie") {
                    if (!subElem->GetText()) {
                        cerr << "Er is een voertuig zonder positie!" << endl;
                        geldig = false;
                        break;
                    }
                    try {
                        voertuig->setPositie(stoi(subElem->GetText()));
                    } catch (exception &) {
                        cerr << "Er is een voertuig waarvan de positie geen integer is!" << endl;
                        geldig = false;
                        break;
                    }
                }
            }
            if (geldig) voertuigen.push_back(voertuig);

        } else if (elementType == "VERKEERSLICHT") {
            Verkeerslicht* verkeerslicht = new Verkeerslicht();
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

                    Baan Verkeerslichtbaan;
                    for (Baan * baan : banen) {
                        if (baan->getNaam() == subElem->GetText()) {
                            Verkeerslichtbaan.setNaam(baan->getNaam());
                            Verkeerslichtbaan.setLengte(baan->getLengte());
                        }
                    }
                    verkeerslicht->setBaan(Verkeerslichtbaan);

                } else if (propertyName == "positie") {
                    if (!subElem->GetText()) {
                        cerr << "Er is een verkeerslicht zonder positie!" << endl;
                        geldig = false;
                        break;
                    }
                    try {
                        verkeerslicht->setPositie(stoi(subElem->GetText()));
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
                    } catch (exception &) {
                        cerr << "Er is een verkeerslicht waarvan de cyclus geen integer is!" << endl;
                        geldig = false;
                        break;
                    }
                }
            } if (geldig) verkeerslichten.push_back(verkeerslicht);

        } else if (elementType == "VOERTUIGGENERATOR") {
            Voertuiggenerator *generator = new Voertuiggenerator();

            for (TiXmlElement *subElem = elem->FirstChildElement(); subElem != nullptr;
                 subElem = subElem->NextSiblingElement()) {
                string propertyName = subElem->Value();

                if (propertyName == "baan" && subElem->GetText()) {
                    generator->setBaan(subElem->GetText());
                } else if (propertyName == "frequentie" && subElem->GetText()) {
                    generator->setFrequentie(stoi(subElem->GetText()));
                }
            }

            voertuiggeneratoren.push_back(generator);
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
    bool consistent = true;

    // Elk voertuig staat op een bestaande baan
    for (Voertuig* const& v : voertuigen) {
        consistent = false;
        for (Baan* const& b : banen) {
            if (v->getBaan().getNaam() == b->getNaam()) {
                consistent = true;
                break;
            }
        }
        if (!consistent) return false;
    }

    // Elk verkeerslicht staat op een bestaande baan
    for (Verkeerslicht* const& v : verkeerslichten) {
        consistent = false;
        for (Baan* const& b : banen) {
            if (v->getBaan().getNaam() == b->getNaam()) {
                consistent = true;
                break;
            }
        }
        if (!consistent) return false;
    }

    // Elke voertuiggenerator staat op een bestaande baan
    for (Voertuiggenerator* const& v : voertuiggeneratoren) {
        consistent = false;
        for (Baan* const& b : banen) {
            if (v->getBaan() == b->getNaam()) {
                consistent = true;
                break;
            }
        }
        if (!consistent) return false;
    }

    // De positie van elk voertuig is kleiner dan de lengte van de baan
    for (Voertuig* const& v : voertuigen) {
        for (Baan* const& b : banen) {
            if (v->getBaan().getNaam() == b->getNaam()) {
                if (v->getPositie() > b->getLengte()) {
                    return false;
                }
                break;
            }
        }
    }

    // De positie van elk verkeerslicht is kleiner dan de lengte van de baan
    for (Verkeerslicht* const& v : verkeerslichten) {
        for (Baan* const& b : banen) {
            if (v->getBaan().getNaam() == b->getNaam()) {
                if (v->getPositie() > b->getLengte()) {
                    return false;
                }
                break;
            }
        }
    }

    // Er is maximaal één voertuiggenerator op elke baan
    map<string, int> vg_op_banen;
    for (Baan* const& b : banen) {
        vg_op_banen[b->getNaam()] = 0;
    }

    for (Voertuiggenerator* const& v : voertuiggeneratoren) {
        vg_op_banen[v->getBaan()] += 1;
    }

    for (const pair<const string, int>& k : vg_op_banen) {
        if (k.second > 1) {
            return false;
        }
    }

    return true;
}

void simulation::ToString() {
    cout << "------------- SIMULATIE " << getSimulationTime()<< " ------------- "  << endl;

    cout << "Tijd: " << getincSimulationTime()  << endl;

    for (Voertuig* voertuig : voertuigen) {
        cout << "Voertuig " << voertuig->getId() << "\n"
        << "-> baan: " << voertuig->getBaan().getNaam() << "\n"
        << "-> positie: " << voertuig->getPositie() << "\n"
        << "-> snelheid: " << voertuig->getSnelheid() << "\n" << endl;
    }

}

double simulation::getSimulationTime() const {
    return simulationTime;
}

double simulation::getincSimulationTime() const {
    return simulationincreasedTime ;
}

double simulation::incSimulationTime() {
    simulationincreasedTime = simulationincreasedTime + simulationTimeinc;
    return simulationincreasedTime ;
}

double simulation::UpdateSimulationTime() const {
    return simulationTime + simulationTimeinc; //TODO fix updatesimulationtime so that it updates somewhere in the program
}



void simulation::sortVoertuigenByPosition() { sort(voertuigen.begin(), voertuigen.end(), [](const Voertuig* a, const Voertuig* b) {return a->getPositie() < b->getPositie();});
}

void simulation::sortVerkeersLichtByPosition() { sort(verkeerslichten.begin(), verkeerslichten.end(), [](const Verkeerslicht* a, const Verkeerslicht* b) {return a->getPositie() < b->getPositie();});
}

void simulation::BerekenPositie(Voertuig* v) const {

    //berekenen van nieuwe positie
    if ((v->getSnelheid() + v->getVersnelling()*simulationTimeinc)<0){
        double newPosition = v->getPositie() - ((v->getSnelheid()*v->getSnelheid())/(v->getVersnelling()*2));
        v->setPositie(newPosition);
        v->setSnelheid(0);
    }
    else {
        double calcsnelheid = (v->getSnelheid()+(v->getVersnelling()*simulationTimeinc));
        v->setSnelheid(calcsnelheid);
        v->setPositie(v->getPositie() + (v->getSnelheid()*simulationTimeinc) + v->getVersnelling()* (pow(simulationTimeinc,2)/2));
    }

}

void simulation::BerekenVersnelling(Voertuig* v, int counter) const {

    double delta = 0;

    size_t vsize = voertuigen.size();
    int sizeVoertuigen = vsize;

    if(sizeVoertuigen > counter + 1) {
        double volgafstand = voertuigen[counter + 1]->getPositie() - v->getPositie() - v->getLength();
        double snelheidVerschil = v->getSnelheid() - voertuigen[counter + 1]->getSnelheid();

        double newsnelheid = v->getSnelheid() - snelheidVerschil;
        double newversnelling = 2 * sqrt(v->getAmax() * v->getBmax()) ;

        double calculate  = v->getSnelheid() + (newsnelheid / newversnelling);

        double maxNummer = max(0.0, calculate);
        delta = (fmin + maxNummer) / volgafstand;

        double newVersnelling = v->getAmax() * (1 - std::pow((v->getSnelheid() / v->getKvmax()), 4) - std::pow(delta, 2));
        v->setVersnelling(newVersnelling);
    } else {
        double newVersnelling = v->getAmax() * (1 - std::pow((v->getSnelheid() / v->getKvmax()), 4) - std::pow(delta, 2));
        v->setVersnelling(newVersnelling);
    }
}

void simulation::UpdateVoertuig(Voertuig* v, int counter) const {
    BerekenPositie(v);
    //TODO: als de voertuig in stop/vertraagzone zit dan moet die vertragen in plaats van versnellen.
    //TODO: vind een manier om de IsVoertuigInStopZone en IsVoertuigInVertraagZone hier te laten werken.
    BerekenVersnelling(v, counter);
}

bool simulation::IsVoertuigOpBaan(Voertuig* v) {
    if (v->getPositie() > v->getBaan().getLengte()) {
        return false;
    }
    return true;
}

void simulation::simulationRun() {

    int counter = 0;
    for (Voertuig* v: voertuigen){
        UpdateVoertuig(v,counter);

        if (!IsVoertuigOpBaan(v)) {
            // Vehicle is no longer on the road, so remove it from the vector and delete it
            __gnu_cxx::__normal_iterator<Voertuig **, vector<Voertuig *>> it = std::find(voertuigen.begin(), voertuigen.end(), v); // Find the iterator for v

            if (it != voertuigen.end()) { // Ensure the element was found
                voertuigen.erase(it); // Remove the element from the vector
                delete v; // Free the memory
            }
        }

        counter ++;
    }
    simulationTime ++;
    incSimulationTime();
}

bool simulation::IsVoertuigInVertraagZone(Voertuig *v, Verkeerslicht* l) {
    if(v->getPositie() - l->getPositie() <= vertraagAfstand){
        return true;
    }

    return false;
}

bool simulation::IsVoertuigInStopZone(Voertuig *v, Verkeerslicht* l) {
    if(v->getPositie()<(l->getPositie()-(stopAfstand/2))){
        return true;
    }
    return false;
}

void simulation::BerekenSnelheidNaVertraging(Voertuig *v)  {
    v->setKvmax(v->getGVmax()*v->getVertraagFactor());
}

void simulation::BerekenSnelheidNaVersnelling(Voertuig *v) {
    v->setKvmax(v->getGVmax());
}

vector<Voertuig*> simulation::VoertuigenTussenVerkeerslichten(Verkeerslicht* lichtVoor, Verkeerslicht* lichtAchter) {
    vector<Voertuig*> VoertuigenVoorVerkeerslicht;
    for (Voertuig* v: voertuigen) {
        // licht en voertuig moeten op dezelfde baan
        if (v->getBaan().getNaam() == lichtVoor->getBaan().getNaam() || lichtAchter->getPositie() < v->getPositie() < lichtVoor->getPositie()) {
            VoertuigenVoorVerkeerslicht.push_back(v);
        }
    }
    return VoertuigenVoorVerkeerslicht;
}

vector <Verkeerslicht*> simulation::VerkeerslichtenOpBaan(Verkeerslicht* licht){
    vector<Verkeerslicht*> VerkeersLichtenOpDezelfdeBaan;
    for (Verkeerslicht* v: verkeerslichten) {
        if (licht->getBaan().getNaam() == v->getBaan().getNaam()) {
            VerkeersLichtenOpDezelfdeBaan.push_back(v);
        }
    }
    return VerkeersLichtenOpDezelfdeBaan;
}


void simulation::UpdateVoertuigenAanVerkeerslichtSituatie() {
    int VerkeerslichtCounter = 0;

    for (Verkeerslicht* licht: verkeerslichten) {
        //  1. IF tijd sinds laatste verandering > cyclus
        if( licht->getTijdSindsLaatsteVerandering() > licht->getCyclus() ){
            licht->UpdateVerkeersLicht();// 1.1 THEN verander de kleur van het licht (groen ↔ rood)
            licht->UpdateTijdSindsLaatsteVerandering(simulationTimeinc);
        }

        vector <Verkeerslicht*> verkeerslichten_baan = VerkeerslichtenOpBaan(licht);

        if (verkeerslichten_baan[VerkeerslichtCounter+1] != nullptr || licht->getBaan().getNaam() == verkeerslichten_baan[VerkeerslichtCounter+1]->getBaan().getNaam()) {
            // Als volgende licht bestaat pak alle voertuigen ertussen en als lichten op dezelfde baan staan
            vector<Voertuig*> VoertuigenVoorLicht = VoertuigenTussenVerkeerslichten(licht, verkeerslichten_baan[VerkeerslichtCounter+1]);

            Voertuig* eerstVoertuigVoorLicht = VoertuigenVoorLicht.at(0);             // pas alleen elementen aan voor eerste auto voor licht de rest volgt automtisch

            if (licht->isGroen()) {                                                     // 2. IF verkeerslicht is groen
                BerekenSnelheidNaVersnelling(eerstVoertuigVoorLicht);                   // 2.1 THEN voertuigen voor het verkeerslicht mag terug versnellen
            }

            else if(licht->isRood()){                                                   // 3.1 ELSE IF verkeerslicht is rood
                if ( IsVoertuigInVertraagZone(eerstVoertuigVoorLicht, licht) ){                   // 3.1.1 THEN IF het eerste voertuig voor het licht bevindt zich in de vertraagafstand
                    BerekenSnelheidNaVertraging(eerstVoertuigVoorLicht);                // 3.1.1.1 THEN pas de vertraagfactor toe op het voertuig
                }
                else if (IsVoertuigInStopZone(eerstVoertuigVoorLicht)){                 // 3.1.2 ELSE IF het eerste voertuig voor het licht bevindt zich in de eerste helft van de stopafstand
                    eerstVoertuigVoorLicht->UpdateVersnellingVoorStoppen();             // 3.1.1.1 THEN laat het voertuig stoppen
                }
            }
        } else if (verkeerslichten[VerkeerslichtCounter+1] == nullptr || licht->getBaan().getNaam() == verkeerslichten[VerkeerslichtCounter+1]->getBaan().getNaam()) {

        }
        VerkeerslichtCounter ++;
    }
}








