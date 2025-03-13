//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#include "Voertuiggenerator.h"


std::string Voertuiggenerator::getBaan() const {
    return baan;
}

void Voertuiggenerator::setBaan(const std::string &baan) {
    this->baan = baan;
}

double Voertuiggenerator::getFrequentie() const {
    return frequentie;
}

void Voertuiggenerator::setFrequentie(const double frequentie) {
    this->frequentie = frequentie;
}
