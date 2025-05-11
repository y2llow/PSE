//
// Created by Abdellah on 5/9/2025.
//

#include "Kruispunt.h"

#include "Baan.h"
#include "Verkeerslicht.h"
#include "../DesignByContract.h"

#include<algorithm>

Kruispunt::Kruispunt(const vector<Baan*>& b, const map<Baan*, int>& p)
        : bannen(b), positions(p), active_verkeerslicht_(nullptr), time_since_last_change(0) {}

const vector<Baan*> Kruispunt::getBannen() const {
    return bannen;
}

const map<Baan*, int> Kruispunt::getPositions() const {
    return positions;
}

void Kruispunt::addBaan(Baan* baan) {
    REQUIRE(baan != nullptr, "addBaan: baan mag niet nullptr zijn");
    bannen.push_back(baan);
}

void Kruispunt::addPosition(Baan* baan, int position) {
    REQUIRE(baan != nullptr, "addPosition: baan mag niet nullptr zijn");
    positions[baan] = position;
}

tuple<bool, vector<Verkeerslicht*>> Kruispunt::verkeerslichtenStaanOpKruispunt() const {
    vector<Verkeerslicht*> lights;
    for (auto [baan, pos] : getPositions()) {
        bool matchFound = false;
        for (const auto v : baan->getVerkeerslichten()) {
            if (v->getPositie() == pos) {
                lights.push_back(v);
                matchFound = true;
                break;
            }
        }
        if (!matchFound)
            return make_tuple(false, vector<Verkeerslicht*>());
    }
    return make_tuple(true, lights);
}

double Kruispunt::getTimeSince() const {
    return time_since_last_change;
}

void Kruispunt::increaseTimeSince(const double i) {
    REQUIRE(i >= 0, "increaseTimeSince: tijd mag niet negatief zijn");
    time_since_last_change += i / (double)getBannen().size();
}

void Kruispunt::setTimeSince(const double t) {
    REQUIRE(t >= 0, "setTimeSince: tijd mag niet negatief zijn");
    time_since_last_change = t;
}

Kruispunt* Kruispunt::createNewKruispunt(const vector<Baan*>& bannen, const map<Baan*, int>& positions) {
    for (auto* baan : bannen)
        REQUIRE(baan != nullptr, "createNewKruispunt: bannen mogen geen nullptr bevatten");

    for (auto [baan, _] : positions)
        REQUIRE(baan != nullptr, "createNewKruispunt: positions mogen geen nullptr bevatten");

    auto *k = new Kruispunt(bannen, positions);
    ENSURE(k != nullptr, "createNewKruispunt: aanmaak gefaald");
    return k;
}

void Kruispunt::setActiveVerkeerslicht(Verkeerslicht* verkeerslicht) {
    REQUIRE(verkeerslicht != nullptr, "setActiveVerkeerslicht: verkeerslicht is nullptr");

    active_verkeerslicht_ = verkeerslicht;
    verkeerslicht->setState(LightState::GREEN);
    verkeerslicht->setTimeSince(0);
    time_since_last_change = 0;

    auto verkeerslichten = get<1>(verkeerslichtenStaanOpKruispunt());

    for (const auto v : verkeerslichten) {
        if (v != active_verkeerslicht_ && v->getState() != LightState::RED) {
            v->setState(LightState::ORANGE);
            v->setTimeSince(0);
            time_since_last_change = 0;
        }
    }
}

Verkeerslicht* Kruispunt::getActiveVerkeerslicht() {
    return active_verkeerslicht_;
}
