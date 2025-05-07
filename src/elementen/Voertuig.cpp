#include "Voertuig.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <ostream>
#include <random>
#include <cassert>

#include "Constants.h"
#include "../simulatie/Simulator.h"
#include "voertuigen/Auto.h"
#include "voertuigen/Brandweerwagen.h"
#include "voertuigen/Bus.h"
#include "voertuigen/Politiecombi.h"
#include "voertuigen/Ziekenwagen.h"

Baan* Voertuig::getBaan() const
{
    assert(properlyInit());
    return baan;
}

void Voertuig::setBaan(Baan* weg)
{
    assert(properlyInit());
    baan = weg;
    assert(getBaan() == weg);
}

double Voertuig::getPositie() const
{
    assert(properlyInit());
    return p;
}

void Voertuig::setPositie(const double positie)
{
    assert(properlyInit());
    p = positie;
    assert(getPositie() == positie);
}

double Voertuig::getSnelheid() const
{
    assert(properlyInit());
    return v;
}

void Voertuig::setSnelheid(const double speed)
{
    assert(properlyInit());
    v = speed;
    assert(getSnelheid() == speed);
}

State Voertuig::getState() const
{
    assert(properlyInit());
    return voertuig_state;
}

int Voertuig::getId() const
{
    assert(properlyInit());
    return id;
}

void Voertuig::setId(const int ID)
{
    assert(properlyInit());
    this->id = ID;
    assert(getId() == ID);
}

double Voertuig::getVersnelling() const
{
    assert(properlyInit());
    return a;
}

void Voertuig::setVersnelling(const double versnelling)
{
    assert(properlyInit());
    a = versnelling;
    assert(getVersnelling() == versnelling);
}

double Voertuig::getKvmax() const
{
    assert(properlyInit());
    return k_v_max;
}

void Voertuig::setKvmax(const double kvmax)
{
    assert(properlyInit());
    k_v_max = kvmax;
    assert(getKvmax() == kvmax);
}

double Voertuig::getLengte() const
{
    assert(properlyInit());
    return l;
}


void Voertuig::setLengte(double lengte)
{
    assert(properlyInit());
    l = lengte;
    assert(getLengte() == lengte);
}

double Voertuig::getMaximaleSnelheid() const
{
    assert(properlyInit());
    return v_max;
}

void Voertuig::setMaximaleSnelheid(const double maximale_snelheid)
{
    assert(properlyInit());
    v_max = maximale_snelheid;
    assert(getMaximaleSnelheid() == maximale_snelheid);
}

double Voertuig::getMaximaleVersnelling() const
{
    assert(properlyInit());
    return a_max;
}

void Voertuig::setMaximaleVersnelling(const double maximale_versnelling)
{
    assert(properlyInit());
    a_max = maximale_versnelling;
    assert(getMaximaleVersnelling() == maximale_versnelling);
}

double Voertuig::getMaximaleRemfactor() const
{
    assert(properlyInit());
    return b_max;
}

void Voertuig::setMaximaleRemfactor(const double maximale_remfactor)
{
    assert(properlyInit());
    b_max = maximale_remfactor;
    assert(getMaximaleRemfactor() == maximale_remfactor);
}

double Voertuig::getMinimaleVolgafstand() const
{
    assert(properlyInit());
    return f_min;
}

void Voertuig::setMinimaleVolgafstand(const double minimale_volgaftand)
{
    assert(properlyInit());
    f_min = minimale_volgaftand;
    assert(getMinimaleVolgafstand() == minimale_volgaftand);
}


bool Voertuig::isPrioriteitsVoertuig() const
{
    assert(properlyInit());
    return prioriteitsvoertuig;
}

string Voertuig::getType() const
{
    assert(properlyInit());
    return "";
}

void Voertuig::setState(const State state)
{
    assert(properlyInit());
    this->voertuig_state = state;
    assert(getState() == state);
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

    //cerr << "Onbekend voertuigtype: " << type << ". Type AUTO wordt gebruikt." << endl;
    return new Auto();
}


void Voertuig::rijd()
{
    assert(properlyInit());
    assert(baan != nullptr);

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
    assert(properlyInit());
    k_v_max = VERTRAAG_FACTOR * v_max;
    voertuig_state = State::SLOWINGDOWN;

    // Postcondities
    assert(getState() == State::SLOWINGDOWN);
    assert(getKvmax() == VERTRAAG_FACTOR * getMaximaleSnelheid());
}

void Voertuig::accelerate()
{
    assert(properlyInit());
    k_v_max = v_max;
    voertuig_state = State::DRIVING;

    // Postcondities
    assert(getState() == State::DRIVING);
    assert(getKvmax() == getMaximaleSnelheid());
}

void Voertuig::stop()
{
    assert(properlyInit());
    a = -(b_max * v / k_v_max);
    voertuig_state = State::STOPPING;

    // Postcondities
    assert(getState() == State::STOPPING);
}

void Voertuig::checkForKruispunt(double position, double newposition)
{
    assert(properlyInit());
    auto kruispunten = baan->getKruispunten();

    for (const auto& k : kruispunten)
    {
        auto KruispuntPosition = k.first;
        // als de auto over het kruispunt is gereden dan chekken we voor een baanwisseling
        if (KruispuntPosition >= position && KruispuntPosition <= newposition)
            kruispunt(k);
    }
}

void Voertuig::kruispunt(pair<int, vector<Baan*>> k)
{
    assert(properlyInit());
    if (!baan) return;

    // check voor kruispunten
    if (!k.second.empty())
    {
        std::vector<Baan*> alle_opties = k.second;
        alle_opties.push_back(baan);

        int random_index = std::rand() % alle_opties.size();
        Baan* selected_road = alle_opties[random_index];

        // Update position
        if (selected_road != baan)
        {
            // Bewaar oude baan voor postcondities
            Baan* oude_baan = baan;

            // verander de positie correct
            for (const auto& [fst, snd] : selected_road->getKruispunten())
            {
                for (auto b : snd)
                {
                    if (b == baan)
                    {
                        p = fst + 1;
                        break;
                    }
                }
            }

            // verander baan
            selected_road->addVoertuig(this);
            baan->takeOutVoertuig(this);
            setBaan(selected_road);

            // Postcondities
            assert(getBaan() == selected_road);
            assert(std::find(selected_road->getVoertuigen().begin(),
                             selected_road->getVoertuigen().end(),
                             this) != selected_road->getVoertuigen().end());
            assert(std::find(oude_baan->getVoertuigen().begin(),
                             oude_baan->getVoertuigen().end(),
                             this) == oude_baan->getVoertuigen().end());
        }
    }
}