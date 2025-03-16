//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#include "Voertuig.h"

#include <cmath>

#include "Constants.h"


string Voertuig::getBaan() const {
    return baan;
}

void Voertuig::setBaan(const string &weg) {
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



/*
void Voertuig::BerekenPositieVoertuig() {

    //berekenen van nieuwe positie
    if ((snelheid + versnelling * SIMULATIE_TIJDinc)<0){
        double newPosition = positie - ((snelheid * snelheid) / (versnelling * 2) );
        setPositie(newPosition);
        setSnelheid(0);
    }
    else {
        setSnelheid((snelheid + (versnelling * SIMULATIE_TIJDinc)));
        setPositie(positie + (snelheid * SIMULATIE_TIJDinc) + versnelling * ((SIMULATIE_TIJDinc * SIMULATIE_TIJDinc) / 2));
    }

}

void Voertuig::BerekenVersnellingVoertuig(std::size_t counter , std::vector<Voertuig*> voertuigen) {

    double delta = 0;

    double volgafstand = voertuigen[counter + 1]->getPositie() - positie - length;
    double snelheidVerschil = snelheid - voertuigen[counter + 1]->getSnelheid();

    double newsnelheid = snelheid - snelheidVerschil;
    double newversnelling = 2 * std::sqrt(amax * bmax) ;

    double calculate  = snelheid + (newsnelheid / newversnelling);

    double maxNummer = std::max(0.0, calculate);
    delta = (fmin + maxNummer) / volgafstand;

    double newVersnelling = amax * (1 - std::pow((snelheid / Vmax), 4) - std::pow(delta, 2));
    setVersnelling(newVersnelling);
}

*/
