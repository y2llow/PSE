//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#include "Voertuig.h"

void Voertuig::setBaan(const std::string &baan) {
    this->baan = baan;
}

int Voertuig::getPositie() const {
    return positie;
}

std::string Voertuig::getBaan() const {
    return baan;
}

void Voertuig::setPositie(const int p) {
    positie = p;
}

