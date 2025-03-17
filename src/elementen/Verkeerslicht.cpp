//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#include "Verkeerslicht.h"
#include <iostream>

string Verkeerslicht::getBaan() const {
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

void Verkeerslicht::setBaan(const string &b) {
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

void Verkeerslicht::updateVerkeersLicht() {
    groen = rood;
    rood = !rood;

    herstartCyclus();
    
    // if (rood) {
    //     groen = true;
    //     rood = false;
    //     herstartCyclus();
    // } else {
    //     rood = true;
    //     groen = false;
    //     herstartCyclus();
    // }
}

void Verkeerslicht::updateTijdSindsLaatsteVerandering(double tijd) {
    tijdSindsLaatsteVerandering += tijd;
}

void Verkeerslicht::herstartCyclus() {
    tijdSindsLaatsteVerandering -= cyclus;
}

double Verkeerslicht::getTijdSindsLaatsteVerandering() const {
    return tijdSindsLaatsteVerandering;
}
