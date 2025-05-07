//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#include "Baan.h"

#include <algorithm>

#include "Verkeerslicht.h"
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

void Baan::addKruispunt(const int key, Baan* value)
{
    kruispunten[key].push_back(value);
}

void Baan::removeVoertuig(Voertuig* v)
{
    voertuigen.erase(remove(voertuigen.begin(), voertuigen.end(), v), voertuigen.end());
    delete v;
}

void Baan::takeOutVoertuig(Voertuig* v)
{
    voertuigen.erase(remove(voertuigen.begin(), voertuigen.end(), v), voertuigen.end());
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

const map<int, vector<Baan*>>& Baan::getKruispunten() const
{
    return kruispunten;
}

void Baan::sortVoertuigenByPosition()
{
    sort(voertuigen.begin(), voertuigen.end(), [](const Voertuig* a, const Voertuig* b)
    {
        return a->getPositie() > b->getPositie();
    });
}

void Baan::sortVerkeerslichtenByPosition()
{
    sort(verkeers.begin(), verkeers.end(), [](const Verkeerslicht* a, const Verkeerslicht* b)
    {
        return a->getPositie() < b->getPositie();
    });
}
