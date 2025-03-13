//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#include "Baan.h"

std::string Baan::getNaam() const {
    return naam;
}

int Baan::getLengte() const {
    return lengte;
}

void Baan::setNaam(const std::string &name) {
    this->naam = name;
}

void Baan::setLengte(const int length) {
    this->lengte = length;
}
