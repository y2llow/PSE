#include "Baan.h"
#include <cassert>
#include <algorithm>
#include "Verkeerslicht.h"
#include "Voertuig.h"

std::string Baan::getNaam() const {
    assert(properlyInit());
    return naam;
}

void Baan::setNaam(const std::string& naam) {
    assert(properlyInit());
    this->naam = naam;
    assert(getNaam() == naam);
}

int Baan::getLengte() const {
    assert(properlyInit());
    return lengte;
}

void Baan::setLengte(const int lengte) {
    assert(properlyInit());
    assert(lengte > 0);
    this->lengte = lengte;
    assert(getLengte() == lengte);
}

void Baan::addBushalte(Bushalte* b) {
    assert(properlyInit());
    assert(b != nullptr);
    bushaltes.push_back(b);
    assert(getBushaltes().back() == b);
}

void Baan::addVerkeerslicht(Verkeerslicht* v) {
    assert(properlyInit());
    assert(v != nullptr);
    verkeers.push_back(v);
    assert(getVerkeerslichten().back() == v);
}

void Baan::addVoertuig(Voertuig* v) {
    assert(properlyInit());
    assert(v != nullptr);
    voertuigen.push_back(v);
    assert(getVoertuigen().back() == v);
}

void Baan::addVoertuiggenerator(Voertuiggenerator* vg) {
    assert(properlyInit());
    assert(vg != nullptr);
    voertuiggeneratoren.push_back(vg);
    assert(getVoertuigeneratoren().back() == vg);
}

void Baan::addKruispunt(const int key, Baan* value) {
    assert(properlyInit());
    assert(value != nullptr);
    kruispunten[key].push_back(value);
    assert(std::find(kruispunten[key].begin(), kruispunten[key].end(), value) != kruispunten[key].end());
}

void Baan::removeVoertuig(Voertuig* v) {
    assert(properlyInit());
    assert(v != nullptr);
    voertuigen.erase(std::remove(voertuigen.begin(), voertuigen.end(), v), voertuigen.end());
    delete v;
    assert(std::find(voertuigen.begin(), voertuigen.end(), v) == voertuigen.end());
}

void Baan::takeOutVoertuig(Voertuig* v) {
    assert(properlyInit());
    assert(v != nullptr);
    voertuigen.erase(std::remove(voertuigen.begin(), voertuigen.end(), v), voertuigen.end());
    assert(std::find(voertuigen.begin(), voertuigen.end(), v) == voertuigen.end());
}

const vector<Verkeerslicht*>& Baan::getVerkeerslichten() const {
    assert(properlyInit());
    return verkeers;
}

const vector<Bushalte*>& Baan::getBushaltes() const {
    assert(properlyInit());
    return bushaltes;
}

const vector<Voertuig*>& Baan::getVoertuigen() const {
    assert(properlyInit());
    return voertuigen;
}

const vector<Voertuiggenerator*>& Baan::getVoertuigeneratoren() const {
    assert(properlyInit());
    return voertuiggeneratoren;
}

const map<int, vector<Baan*>>& Baan::getKruispunten() const {
    assert(properlyInit());
    return kruispunten;
}

void Baan::sortVoertuigenByPosition() {
    assert(properlyInit());
    sort(voertuigen.begin(), voertuigen.end(), [](const Voertuig* a, const Voertuig* b) {
        return a->getPositie() > b->getPositie();
    });
    for (size_t i = 0; i + 1 < voertuigen.size(); ++i) {
        assert(voertuigen[i]->getPositie() >= voertuigen[i + 1]->getPositie());
    }
}

void Baan::sortVerkeerslichtenByPosition() {
    assert(properlyInit());
    sort(verkeers.begin(), verkeers.end(), [](const Verkeerslicht* a, const Verkeerslicht* b) {
        return a->getPositie() < b->getPositie();
    });
    for (size_t i = 0; i + 1 < verkeers.size(); ++i) {
        assert(verkeers[i]->getPositie() <= verkeers[i + 1]->getPositie());
    }
}
