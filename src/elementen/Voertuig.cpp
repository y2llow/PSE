#include "Voertuig.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <ostream>
#include <random>

#include "Constants.h"
#include "../simulatie/Simulator.h"
#include "voertuigen/Auto.h"
#include "voertuigen/Brandweerwagen.h"
#include "voertuigen/Bus.h"
#include "voertuigen/Politiecombi.h"
#include "voertuigen/Ziekenwagen.h"

Baan* Voertuig::getBaan() const
{
    return baan;
}

void Voertuig::setBaan(Baan* weg)
{
    baan = weg;
}

double Voertuig::getPositie() const
{
    return p;
}

void Voertuig::setPositie(const double positie)
{
    p = positie;
}

double Voertuig::getSnelheid() const
{
    return v;
}

void Voertuig::setSnelheid(const double speed)
{
    v = speed;
}

State Voertuig::getState() const
{
    return voertuig_state;
}

int Voertuig::getId() const
{
    return id;
}

void Voertuig::setId(const int ID)
{
    this->id = ID;
}

double Voertuig::getVersnelling() const
{
    return a;
}

void Voertuig::setVersnelling(const double versnelling)
{
    a = versnelling;
}

double Voertuig::getKvmax() const
{
    return k_v_max;
}

void Voertuig::setKvmax(const double kvmax)
{
    k_v_max = kvmax;
}

double Voertuig::getLengte() const
{
    return l;
}


void Voertuig::setLengte(double lengte)
{
    l = lengte;
}

double Voertuig::getMaximaleSnelheid() const
{
    return v_max;
}

void Voertuig::setMaximaleSnelheid(const double maximale_snelheid)
{
    v_max = maximale_snelheid;
}

double Voertuig::getMaximaleVersnelling() const
{
    return a_max;
}

void Voertuig::setMaximaleVersnelling(const double maximale_versnelling)
{
    a_max = maximale_versnelling;
}

double Voertuig::getMaximaleRemfactor() const
{
    return b_max;
}

void Voertuig::setMaximaleRemfactor(const double maximale_remfactor)
{
    b_max = maximale_remfactor;
}

double Voertuig::getMinimaleVolgafstand() const
{
    return f_min;
}

void Voertuig::setMinimaleVolgafstand(const double minimale_volgaftand)
{
    f_min = minimale_volgaftand;
}


bool Voertuig::isPrioriteitsVoertuig() const
{
    return prioriteitsvoertuig;
}

string Voertuig::getType() const
{
    return "";
}

void Voertuig::setState(const State state)
{
    this->voertuig_state = state;
}


Voertuig* Voertuig::createVoertuig(const string& type)
{
    if (type == "auto")
        return new Auto();
    if (type == "ziekenwagen")
        return new Ziekenwagen();
    if (type == "bus")
        return new Bus();
    if (type == "brandweerwagen")
        return new Brandweerwagen();
    if (type == "politiecombi")
        return new Politiecombi();

    cerr << "Onbekend voertuigtype: " << type << ". Type AUTO wordt gebruikt." << endl;
    return new Auto();
}


void Voertuig::rijd()
{
    if (voertuig_state == State::DRIVING || voertuig_state == State::SLOWINGDOWN)
    {
        // ============= 1. Bereken nieuwe snelheid en positie van voertuig =============
        // ======= Indien v + a∆t kleiner is dan nul =======
        if (const double new_velocity = v + a * SIMULATIE_TIJD; new_velocity < 0)
        {
            // ===== In dit geval passen we de positie aan als volgt =====
            p = p - pow(v, 2) / (2 * a);
            // ===== Vervolgens zetten we de snelheid gelijk aan nul =====
            v = 0;
        }
        else
        {
            // ===== Indien dit niet het geval is, dan passen we eerst de snelheid aan ====
            v = new_velocity;
            // ===== Vervolgens passen we de positie aan =====
            p = p + v * SIMULATIE_TIJD + a * pow(SIMULATIE_TIJD, 2) / 2;
        }

        // ============= 2. Bereken nieuwe versnelling van voertuig =============
        double delta;

        const Voertuig* front_voertuig = baan->getVoertuigen().front();
        for (const auto v : baan->getVoertuigen())
        {
            if (v == this) break;
            front_voertuig = v;
        }

        if (this == front_voertuig || front_voertuig == nullptr)
            delta = 0;
        else
        {
            const double volgafstand = front_voertuig->getPositie() - p - front_voertuig->getLengte();
            const double snelheidsverschil = v - front_voertuig->getSnelheid();

            delta = (f_min + max(0.0, v + v * snelheidsverschil / (2 * sqrt(a_max * b_max)))) / volgafstand;
        }

        a = a_max * (1 - pow(v / k_v_max, 4) - pow(delta, 2));
    }



    // ============= 3. IF nieuwe positie valt buiten huidige baan =============
    if (p > baan->getLengte())
        // ======== 3.1. Verwijder voertuig uit simulatie ========
        baan->removeVoertuig(this);
}

void Voertuig::slowDown()
{
    k_v_max = VERTRAAG_FACTOR * v_max;
    voertuig_state = State::SLOWINGDOWN;
}

void Voertuig::accelerate()
{
    k_v_max = v_max;
    voertuig_state = State::DRIVING;
}

void Voertuig::stop()
{
    a = -(b_max * v / k_v_max);
    voertuig_state = State::STOPPING;
}

void Voertuig::kruispunt(const pair<const int, vector<Baan*>> & k)
{
    Baan* currentBaan = getBaan();
    const auto kruispunten = currentBaan->getKruispunten();

//    for (auto &k : kruispunten){
        // randomise the baan als ze op het kruispunt zijn en als het kruispunt niet op het einde ligt van een baan
        if (std::rand() % 2 == 0) {
            setBaan(k.second[0]);
            k.second[0]->addVoertuig(this);
            currentBaan->TakeOutVoertuig(this);
            
            // get the position of the kruistpunten to update postion of car on new road
            auto map = k.second[0]->getKruispunten();
            for (auto &B: map) {
                p = B.first;
            }
        }
//    }
}

void Voertuig::checkForKruispunt(double position, double newposition) {
    Baan* currentBaan = getBaan();
    auto kruispunten = currentBaan->getKruispunten();

    for (const auto &k : kruispunten)
    {
        auto KruispuntPosition = k.first;
        // als de auto over het kruispunt is gereden dan chekken we voor een baanwisseling
        if (KruispuntPosition >= position && KruispuntPosition <= newposition)
            kruispunt(k);
    }
}

void Voertuig::Kruispunt() {
    Baan* currentBaan = getBaan();
    if (!currentBaan) return;

    double positie = getPositie();
    rijd();
    double newpositie = getPositie();

    for (auto& k : currentBaan->getKruispunten())
    {
        double kruistpunt_pos = k.first;

        // Kijk ofdat we over het krijspunt zijn gegaan tijdens de update
        if ((positie < kruistpunt_pos && newpositie >= kruistpunt_pos) ||
            (positie > kruistpunt_pos && newpositie <= kruistpunt_pos))
        {

            // check voor kruispunten
            if (!k.second.empty())
            {
                std::vector<Baan*> alle_opties = k.second;
                alle_opties.push_back(currentBaan);

                int random_index = std::rand() % alle_opties.size();
                Baan* selected_road = alle_opties[random_index];

                if (selected_road == nullptr){break;}

                // Update position
                if (selected_road != currentBaan)
                {
                    // verander de positie correct
                    for (auto& intersection : selected_road->getKruispunten()) { p = intersection.first; }

                    // verander baan
                    selected_road->addVoertuig(this);
                    currentBaan->TakeOutVoertuig(this);
                    setBaan(selected_road);
                }
            }
            break;
        }
    }
}