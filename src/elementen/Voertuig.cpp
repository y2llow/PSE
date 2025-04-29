#include "Voertuig.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <ostream>

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

int Voertuig::getId() const
{
    return id;
}

void Voertuig::setId(const int id)
{
    this->id = id;
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

bool Voertuig::isSlowed() const
{
    return is_slowed;
}

void Voertuig::setSlowed(bool is_slowed)
{
    this->is_slowed = is_slowed;
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

    if (voertuig_state == State::DRIVING)
        a = a_max * (1 - pow(v / k_v_max, 4) - pow(delta, 2));
    else
        a = -(b_max * v / k_v_max);


    // ============= 3. IF nieuwe positie valt buiten huidige baan =============
    if (p > baan->getLengte())
        // ======== 3.1. Verwijder voertuig uit simulatie ========
        baan->removeVoertuig(this);
}

void Voertuig::slowDown()
{
    k_v_max = VERTRAAG_FACTOR * v_max;
    voertuig_state = State::DRIVING;
    is_slowed = true;
}

void Voertuig::accelerate()
{
    k_v_max = v_max;
    voertuig_state = State::DRIVING;
    is_slowed = false;
}

void Voertuig::stop()
{
    voertuig_state = State::STOPPING;
}

void Voertuig::updateVersnellingVoorStoppen() {
    // Gebruik de specifieke maxRemfactor voor dit type voertuig
    a = (-(b_max * v) / k_v_max);

    // Pas versnelling aan op basis van de simulatie constanten indien nodig
    // Dit is een voorbeeld en kan aangepast worden naar de exacte logica die je nodig hebt
    // bijvoorbeeld rekening houdend met VERTRAAG_AFSTAND, STOP_AFSTAND, etc.
}

/*#include "Voertuig.h"

#include <cmath>

#include "Constants.h"

Baan *Voertuig::getBaan() const {
    return baan;
}

void Voertuig::setBaan(Baan *weg) {
    baan = weg;
}

double Voertuig::getPositie() const {
    return positie;
}

void Voertuig::setPositie(double position) {
    positie = position;
}

double Voertuig::getSnelheid() const {
    return snelheid;
}

void Voertuig::setSnelheid(double speed) {
    snelheid = speed;
}

int Voertuig::getId() const {
    return id;
}

void Voertuig::setId(int car_id) {
    id = car_id;
}

double Voertuig::getVersnelling() const {
    return versnelling;
}

void Voertuig::setVersnelling(double acceleration) {
    versnelling = acceleration;
}

double Voertuig::getKvmax() const {
    return kvmax;
}

void Voertuig::setKvmax(double kvm) {
    Voertuig::kvmax = kvm;
}

void Voertuig::UpdateVersnellingVoorStoppen() {
    // Gebruik de specifieke maxRemfactor voor dit type voertuig
    versnelling = (-(getMaxRemfactor() * snelheid) / kvmax);

    // Pas versnelling aan op basis van de simulatie constanten indien nodig
    // Dit is een voorbeeld en kan aangepast worden naar de exacte logica die je nodig hebt
    // bijvoorbeeld rekening houdend met VERTRAAG_AFSTAND, STOP_AFSTAND, etc.
}



//// Implementatie van de simulateStep methode, gebruik makend van de type-specifieke constanten
//void Voertuig::simulateStep() {
//    // Update positie op basis van huidige snelheid en de simulatietijd
//    positie += snelheid * SIMULATIE_TIJD;
//
//    // Update snelheid op basis van versnelling en de simulatietijd
//    snelheid += versnelling * SIMULATIE_TIJD;
//
//    // Zorg ervoor dat de snelheid niet negatief wordt
//    if (snelheid < 0) {
//        snelheid = 0;
//    }
//
//    // Zorg ervoor dat de snelheid niet boven de maximale snelheid voor dit type voertuig komt
//    double maxSnelheid = getMaxSnelheid();
//    if (snelheid > maxSnelheid) {
//        snelheid = maxSnelheid;
//    }
//
//    // Hier kan extra logica worden toegevoegd voor voertuiggedrag
//    // zoals het aanpassen van versnelling op basis van afstand tot voorligger, etc.
//}*/