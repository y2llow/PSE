//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#include "Verkeerslicht.h"

std::string Verkeerslicht::getBaan() const {
    return baan;
}

void Verkeerslicht::setBaan(const std::string &baan) {
    this->baan = baan;
}

int Verkeerslicht::getPositie() const {
    return positie;
}

void Verkeerslicht::setPositie(const int positie) {
    this->positie = positie;
}

int Verkeerslicht::getCyclus() const {
    return cyclus;
}

void Verkeerslicht::setCyclus(const int cyclus) {
    this->cyclus = cyclus;
}
