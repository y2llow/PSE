#include "Voertuig.h"

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
//}