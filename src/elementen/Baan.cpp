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

void Baan::setNaam(const std::string &naam) {
    this->naam = naam;
}

void Baan::setLengte(const int lengte) {
    this->lengte = lengte;
}
