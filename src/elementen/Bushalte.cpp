//
// Created by s0243673 on 24/04/2025.
//

#include "Bushalte.h"



Baan* Bushalte::getBaan() const {
    return baan;
}

void Bushalte::setBaan(Baan* b) {
    this->baan = b;
}

void Bushalte::setPositie(const double position) {
    this->positie = position;
}

double Bushalte::getPositie() const {
    return positie;
}

double Bushalte::getWachttijd() const {
    return wachttijd;
}

void Bushalte::setWachttijd(const double wt) {
    this->wachttijd = wt;
}