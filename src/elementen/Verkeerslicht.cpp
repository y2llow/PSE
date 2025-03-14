//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#include "Verkeerslicht.h"

Baan Verkeerslicht::getBaan() const {
    return baan;
}


void Verkeerslicht::setPositie(const double position) {
    this->positie = position;
}

double Verkeerslicht::getCyclus() const {
    return cyclus;
}

void Verkeerslicht::setCyclus(const double cyc) {
    this->cyclus = cyc;
}

void Verkeerslicht::setBaan(const Baan &b) {
    this->baan = b;
}

double Verkeerslicht::getPositie() const {
    return positie;
}

bool Verkeerslicht::isRood() const {
    return rood;
}

void Verkeerslicht::setRood(bool red) {
    Verkeerslicht::rood = red;
}

bool Verkeerslicht::isGroen() const {
    return groen;
}

void Verkeerslicht::setGroen(bool green) {
    Verkeerslicht::groen = green;
}

