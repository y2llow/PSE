//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#include "Baan.h"

#include <algorithm>

#include "Voertuig.h"

std::string Baan::getNaam() const
{
    return naam;
}

void Baan::setNaam(const std::string& naam)
{
    this->naam = naam;
}

int Baan::getLengte() const
{
    return lengte;
}

void Baan::setLengte(const int lengte)
{
    this->lengte = lengte;
}

void Baan::addBushalte(Bushalte* b)
{
    bushaltes.push_back(b);
}

void Baan::addVerkeerslicht(Verkeerslicht* v)
{
    verkeers.push_back(v);
}

void Baan::addVoertuig(Voertuig* v)
{
    voertuigen.push_back(v);
}

void Baan::addVoertuiggenerator(Voertuiggenerator* vg)
{
    voertuiggeneratoren.push_back(vg);
}

void Baan::removeVoertuig(Voertuig* v)
{
    voertuigen.erase(remove(voertuigen.begin(), voertuigen.end(), v), voertuigen.end());
    delete v;
}

void Baan::TakeOutVoertuig(Voertuig* v) {
    auto it = std::find(voertuigen.begin(), voertuigen.end(), v);
    if (it != voertuigen.end()) {
        voertuigen.erase(it);  // Removes the pointer from the vector (does NOT delete the object)
    }
}

const vector<Verkeerslicht*>& Baan::getVerkeerslichten() const
{
    return verkeers;
}

const vector<Bushalte*>& Baan::getBushaltes() const
{
    return bushaltes;
}

const vector<Voertuig*>& Baan::getVoertuigen() const
{
    return voertuigen;
}

const vector<Voertuiggenerator*>& Baan::getVoertuigeneratoren() const
{
    return voertuiggeneratoren;
}

void Baan::sortVoertuigenByPosition()
{
    sort(voertuigen.begin(), voertuigen.end(), [](const Voertuig* a, const Voertuig* b)
    {
        return a->getPositie() > b->getPositie();
    });
}
