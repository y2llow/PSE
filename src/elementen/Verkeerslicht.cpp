//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#include "Verkeerslicht.h"

#include <iostream>


#include "Constants.h"
#include "Kruispunt.h"
#include "../simulatie/Simulator.h"


Baan* Verkeerslicht::getBaan() const
{
    return baan;
}


void Verkeerslicht::setPositie(const double positie)
{
    this->positie = positie;
}

Kruispunt* Verkeerslicht::getKruispunt() const
{
    return kruispunt;
}

void Verkeerslicht::setKruispunt(Kruispunt* kruispunt)
{
    this->kruispunt = kruispunt;
}

double Verkeerslicht::getCyclus() const
{
    return cyclus;
}

void Verkeerslicht::setCyclus(const double cyclus)
{
    this->cyclus = cyclus;
}


LightState Verkeerslicht::getState() const
{
    return state;
}

void Verkeerslicht::setState(const LightState state)
{
    this->state = state;
}

void Verkeerslicht::switchColor()
{
    if (state == LightState::GREEN)
        state = LightState::ORANGE;
    else if (state == LightState::ORANGE)
        state = LightState::RED;
    else
        state = LightState::GREEN;
}

int Verkeerslicht::getWaitingVehicles() const
{
    int vehicles_that_wait = 0;
    for (const auto v : baan->getVoertuigen())
    {
        if (positie - v->getPositie() > 0 && positie - v->getPositie() < 50)
        {
            vehicles_that_wait++;
        }
    }

    return vehicles_that_wait;
}

void Verkeerslicht::setBaan(Baan* baan)
{
    this->baan = baan;
}

double Verkeerslicht::getPositie() const
{
    return positie;
}


void Verkeerslicht::updateVerkeerslicht()
{
    tijd_sinds_laatste_verandering += SIMULATIE_TIJD;

    if ((state == LightState::ORANGE && tijd_sinds_laatste_verandering >
        cyclus * .1))
    {
        switchColor();
        tijd_sinds_laatste_verandering = 0;
    }
    if ((tijd_sinds_laatste_verandering > cyclus) && (kruispunt == nullptr))
    {
        switchColor();
        tijd_sinds_laatste_verandering = 0;
    }

    auto voertuigen = baan->getVoertuigen();

    if (state == LightState::GREEN)
    {
        for (const auto v : voertuigen)
        {
            if (positie - v->getPositie() > 0 && positie - v->getPositie() <= 50)
                v->accelerate();
        }
    }
    else
    {
        Voertuig* eerst_voertuig = nullptr;

        for (const auto v : voertuigen)
        {
            if (v->getPositie() < positie)
            {
                eerst_voertuig = v;
                break;
            }
        }

        if (eerst_voertuig == nullptr)
            return;

        const double afstand_van_licht = positie - eerst_voertuig->getPositie();

        if (afstand_van_licht > VERTRAAG_AFSTAND)
            return;

        // ======== IF het eerste voertuig voor het verkeerslicht is een prioriteitsvoertuig ========
        if (eerst_voertuig->isPrioriteitsVoertuig())
        {
            // ======= THEN voertuig hoeft niet te vertragen of te stoppen =======
            return;
        }

        if (afstand_van_licht > STOP_AFSTAND && afstand_van_licht < VERTRAAG_AFSTAND &&
            eerst_voertuig->getState() != State::SLOWINGDOWN && eerst_voertuig->getState() != State::STOPPING)
            eerst_voertuig->slowDown();

        else if (afstand_van_licht < 1 && state != LightState::ORANGE) // afstand_van_licht < STOP_AFSTAND
            eerst_voertuig->stop();
    }
}

void Verkeerslicht::setTimeSince(int i)
{
    tijd_sinds_laatste_verandering = i;
}

double Verkeerslicht::getTimeSince()
{
    return tijd_sinds_laatste_verandering;
}
