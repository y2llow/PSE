//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#include "Voertuig.h"


std::string Voertuig::getBaan() const {
    return baan;
}

void Voertuig::setBaan(const std::string &weg) {
    Voertuig::baan = weg;
}

double Voertuig::getPositie() const {
    return positie;
}

void Voertuig::setPositie(double position) {
    Voertuig::positie = position;
}

double Voertuig::getSnelheid() const {
    return snelheid;
}

void Voertuig::setSnelheid(double speed) {
    Voertuig::snelheid = speed;
}

int Voertuig::getId() const {
    return id;
}

void Voertuig::setId(int car_id) {
    Voertuig::id = car_id;
}

double Voertuig::getVersnelling() const {
    return versnelling;
}

void Voertuig::setVersnelling(double acceleration) {
    Voertuig::versnelling = acceleration;
}

double Voertuig::getLength() const {
    return length;
}


