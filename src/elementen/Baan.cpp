#include "Baan.h"
#include "../DesignByContract.h"
#include <algorithm>
#include "Verkeerslicht.h"
#include "Voertuig.h"

std::string Baan::getNaam() const {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    return naam;
}

void Baan::setNaam(const std::string& naam) {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    this->naam = naam;
    ENSURE(getNaam() == naam, "Naam is niet correct ingesteld");
}

int Baan::getLengte() const {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    return lengte;
}

void Baan::setLengte(const int lengte) {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    REQUIRE(lengte > 0, "Lengte moet groter zijn dan 0");
    this->lengte = lengte;
    ENSURE(getLengte() == lengte, "Lengte is niet correct ingesteld");
}

void Baan::addBushalte(Bushalte* b) {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    REQUIRE(b != nullptr, "Bushalte mag niet nullptr zijn");
    bushaltes.push_back(b);
    ENSURE(getBushaltes().back() == b, "Bushalte is niet correct toegevoegd");
}

void Baan::addVerkeerslicht(Verkeerslicht* v) {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    REQUIRE(v != nullptr, "Verkeerslicht mag niet nullptr zijn");
    verkeers.push_back(v);
    ENSURE(getVerkeerslichten().back() == v, "Verkeerslicht is niet correct toegevoegd");
}

void Baan::addVoertuig(Voertuig* v) {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    REQUIRE(v != nullptr, "Voertuig mag niet nullptr zijn");
    voertuigen.push_back(v);
    ENSURE(getVoertuigen().back() == v, "Voertuig is niet correct toegevoegd");
}

void Baan::addVoertuiggenerator(Voertuiggenerator* vg) {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    REQUIRE(vg != nullptr, "Voertuiggenerator mag niet nullptr zijn");
    voertuiggeneratoren.push_back(vg);
    ENSURE(getVoertuigeneratoren().back() == vg, "Voertuiggenerator is niet correct toegevoegd");
}

void Baan::addKruispunt(const int key, Baan* value) {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    REQUIRE(value != nullptr, "Kruispunt-baan mag niet nullptr zijn");
    kruispunten[key].push_back(value);
    ENSURE(std::find(kruispunten[key].begin(), kruispunten[key].end(), value) != kruispunten[key].end(),
           "Kruispunt is niet correct toegevoegd");
}

void Baan::removeVoertuig(Voertuig* v) {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    REQUIRE(v != nullptr, "Voertuig mag niet nullptr zijn");
    voertuigen.erase(std::remove(voertuigen.begin(), voertuigen.end(), v), voertuigen.end());
    delete v;
    ENSURE(std::find(voertuigen.begin(), voertuigen.end(), v) == voertuigen.end(),
           "Voertuig is niet correct verwijderd");
}

void Baan::takeOutVoertuig(Voertuig* v) {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    REQUIRE(v != nullptr, "Voertuig mag niet nullptr zijn");
    voertuigen.erase(std::remove(voertuigen.begin(), voertuigen.end(), v), voertuigen.end());
    ENSURE(std::find(voertuigen.begin(), voertuigen.end(), v) == voertuigen.end(),
           "Voertuig is niet correct uitgenomen");
}

const vector<Verkeerslicht*>& Baan::getVerkeerslichten() const {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    return verkeers;
}

const vector<Bushalte*>& Baan::getBushaltes() const {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    return bushaltes;
}

const vector<Voertuig*>& Baan::getVoertuigen() const {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    return voertuigen;
}

const vector<Voertuiggenerator*>& Baan::getVoertuigeneratoren() const {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    return voertuiggeneratoren;
}

const map<int, vector<Baan*>>& Baan::getKruispunten() const {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    return kruispunten;
}

void Baan::sortVoertuigenByPosition() {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    sort(voertuigen.begin(), voertuigen.end(), [](const Voertuig* a, const Voertuig* b) {
        return a->getPositie() > b->getPositie();
    });

    // Postconditie: voertuigen zijn gesorteerd op aflopende positie
    for (size_t i = 0; i + 1 < voertuigen.size(); ++i) {
        ENSURE(voertuigen[i]->getPositie() >= voertuigen[i + 1]->getPositie(),
               "Voertuigen zijn niet correct gesorteerd op positie (aflopend)");
    }
}

void Baan::sortVerkeerslichtenByPosition() {
    REQUIRE(properlyInit(), "Baan is niet correct geïnitialiseerd");
    sort(verkeers.begin(), verkeers.end(), [](const Verkeerslicht* a, const Verkeerslicht* b) {
        return a->getPositie() < b->getPositie();
    });

    // Postconditie: verkeerslichten zijn gesorteerd op oplopende positie
    for (size_t i = 0; i + 1 < verkeers.size(); ++i) {
        ENSURE(verkeers[i]->getPositie() <= verkeers[i + 1]->getPositie(),
               "Verkeerslichten zijn niet correct gesorteerd op positie (oplopend)");
    }
}