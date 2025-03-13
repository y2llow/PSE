//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#include "Verkeerslicht.h"

std::string Verkeerslicht::getBaan() const {
    return baan;
}


void Verkeerslicht::setPositie(const double positie) {
    this->positie = positie;
}

double Verkeerslicht::getCyclus() const {
    return cyclus;
}

void Verkeerslicht::setCyclus(const double cyclus) {
    this->cyclus = cyclus;
}

void Verkeerslicht::setBaan(const std::string &baan) {
    this->baan = baan;
}

double Verkeerslicht::getPositie() const {
    return positie;
}

