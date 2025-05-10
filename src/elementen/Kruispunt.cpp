//
// Created by Abdellah on 5/9/2025.
//

#include "Kruispunt.h"

#include "Baan.h"
#include "Verkeerslicht.h"

#include<algorithm>

const vector<Baan*> Kruispunt::getBannen() const
{
    return bannen;
}

const map<Baan*, int> Kruispunt::getPositions() const
{
    return positions;
}

void Kruispunt::addBaan(Baan* baan)
{
    bannen.push_back(baan);
}

void Kruispunt::addPosition(Baan* baan, int position)
{
    positions[baan] = position;
}

// this method checks if all the bannen of this kruispunt have a verkeerslicht on this kruispunt
tuple<bool, vector<Verkeerslicht*>> Kruispunt::verkeerslichtenStaanOpKruispunt() const
{
    vector<Verkeerslicht*> lights;
    for (auto [baan, pos] : getPositions())
    {
        bool matchFound = false;
        for (const auto v : baan->getVerkeerslichten())
        {
            if (v->getPositie() == pos)
            {
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

double Kruispunt::getTimeSince() const
{
    return time_since_last_change;
}

void Kruispunt::increaseTimeSince(const double i)
{
    time_since_last_change += i / getBannen().size();
}

void Kruispunt::setTimeSince(const double t)
{
    time_since_last_change = t;
}


Kruispunt* Kruispunt::createNewKruispunt(const vector<Baan*>& bannen, const map<Baan*, int>& positions)
{
    return new Kruispunt(bannen, positions);
}

void Kruispunt::setActiveVerkeerslicht(Verkeerslicht* verkeerslicht)
{
    active_verkeerslicht_ = verkeerslicht;

    verkeerslicht->setState(LightState::GREEN);
    verkeerslicht->setCyclus(1000);

    auto kruispunten = get<1>(verkeerslichtenStaanOpKruispunt());

    for (const auto k : kruispunten)
    {
        if (k != active_verkeerslicht_)
        {
            k->setState(LightState::RED);
            k->setCyclus(1000);
        }
    }
}

Verkeerslicht* Kruispunt::getActiveVerkeerslicht()
{
    return active_verkeerslicht_;
}
