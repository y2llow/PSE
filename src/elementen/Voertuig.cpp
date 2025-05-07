#include "Voertuig.h"
#include "../DesignByContract.h"

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
    ENSURE(getBaan() == weg, "Baan is niet correct ingesteld");
}

double Voertuig::getPositie() const
{
    return p;
}

void Voertuig::setPositie(const double positie)
{
    p = positie;
    ENSURE(getPositie() == positie, "Positie is niet correct ingesteld");
}

double Voertuig::getSnelheid() const
{
    return v;
}

void Voertuig::setSnelheid(const double speed)
{
    v = speed;
    ENSURE(getSnelheid() == speed, "Snelheid is niet correct ingesteld");
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
    ENSURE(getId() == ID, "ID is niet correct ingesteld");
}

double Voertuig::getVersnelling() const
{
    return a;
}

void Voertuig::setVersnelling(const double versnelling)
{
    a = versnelling;
    ENSURE(getVersnelling() == versnelling, "Versnelling is niet correct ingesteld");
}

double Voertuig::getKvmax() const
{
    return k_v_max;
}

void Voertuig::setKvmax(const double kvmax)
{
    k_v_max = kvmax;
    ENSURE(getKvmax() == kvmax, "Kvmax is niet correct ingesteld");
}

double Voertuig::getLengte() const
{
    return l;
}


void Voertuig::setLengte(double lengte)
{
    l = lengte;
    ENSURE(getLengte() == lengte, "Lengte is niet correct ingesteld");
}

double Voertuig::getMaximaleSnelheid() const
{
    return v_max;
}

void Voertuig::setMaximaleSnelheid(const double maximale_snelheid)
{
    v_max = maximale_snelheid;
    ENSURE(getMaximaleSnelheid() == maximale_snelheid, "Maximale snelheid is niet correct ingesteld");
}

double Voertuig::getMaximaleVersnelling() const
{
    return a_max;
}

void Voertuig::setMaximaleVersnelling(const double maximale_versnelling)
{
    a_max = maximale_versnelling;
    ENSURE(getMaximaleVersnelling() == maximale_versnelling, "Maximale versnelling is niet correct ingesteld");
}

double Voertuig::getMaximaleRemfactor() const
{
    return b_max;
}

void Voertuig::setMaximaleRemfactor(const double maximale_remfactor)
{
    b_max = maximale_remfactor;
    ENSURE(getMaximaleRemfactor() == maximale_remfactor, "Maximale remfactor is niet correct ingesteld");
}

double Voertuig::getMinimaleVolgafstand() const
{
    return f_min;
}

void Voertuig::setMinimaleVolgafstand(const double minimale_volgaftand)
{
    f_min = minimale_volgaftand;
    ENSURE(getMinimaleVolgafstand() == minimale_volgaftand, "Minimale volgafstand is niet correct ingesteld");
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
    ENSURE(getState() == state, "State is niet correct ingesteld");
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
    REQUIRE(baan != nullptr, "Baan mag niet nullptr zijn");

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

    // Postcondities
    ENSURE(getState() == State::SLOWINGDOWN, "State is niet correct ingesteld op SLOWINGDOWN");
    ENSURE(getKvmax() == VERTRAAG_FACTOR * getMaximaleSnelheid(), "Kvmax is niet correct aangepast voor vertraging");
}

void Voertuig::accelerate()
{
    k_v_max = v_max;
    voertuig_state = State::DRIVING;

    // Postcondities
    ENSURE(getState() == State::DRIVING, "State is niet correct ingesteld op DRIVING");
    ENSURE(getKvmax() == getMaximaleSnelheid(), "Kvmax is niet correct hersteld na versnelling");
}

void Voertuig::stop()
{
    a = -(b_max * v / k_v_max);
    voertuig_state = State::STOPPING;

    // Postcondities
    ENSURE(getState() == State::STOPPING, "State is niet correct ingesteld op STOPPING");
}

void Voertuig::checkForKruispunt(double position, double newposition)
{
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
            ENSURE(getBaan() == selected_road, "Baan is niet correct gewijzigd na kruispunt");
            ENSURE(std::find(selected_road->getVoertuigen().begin(),
                             selected_road->getVoertuigen().end(),
                             this) != selected_road->getVoertuigen().end(),
                   "Voertuig is niet correct toegevoegd aan de nieuwe baan");
            ENSURE(std::find(oude_baan->getVoertuigen().begin(),
                             oude_baan->getVoertuigen().end(),
                             this) == oude_baan->getVoertuigen().end(),
                   "Voertuig is niet correct verwijderd uit de oude baan");
        }
    }
}