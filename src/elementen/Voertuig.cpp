//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

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
    versnelling = (-(MAX_REMFACTOR * snelheid) / kvmax);
}
