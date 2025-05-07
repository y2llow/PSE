//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#include "Verkeerslicht.h"

#include <assert.h>
#include <cmath>
#include <iostream>

#include "Constants.h"
#include "../simulatie/Simulator.h"
#include "../DesignByContract.h"


Baan* Verkeerslicht::getBaan() const
{
    assert(properlyInit());
    return baan;
}


void Verkeerslicht::setPositie(const double positie)
{
    assert(properlyInit());
    this->positie = positie;
    assert(getPositie() == positie);
}

double Verkeerslicht::getCyclus() const
{
    assert(properlyInit());
    return cyclus;
}

void Verkeerslicht::setCyclus(const double cyclus)
{
    assert(properlyInit());
    assert(cyclus > 0);
    this->cyclus = cyclus;
    assert(getCyclus() == cyclus);
}

void Verkeerslicht::setBaan(Baan* baan)
{
    assert(properlyInit());
    this->baan = baan;
    assert(getBaan() == baan);
}

double Verkeerslicht::getPositie() const
{
    assert(properlyInit());
    return positie;
}

bool Verkeerslicht::isGroen() const
{
    assert(properlyInit());
    return groen;
}

void Verkeerslicht::switchColor()
{
    assert(properlyInit());
    bool oude_waarde = groen;
    groen = !groen;
    assert(isGroen() != oude_waarde);
}

void Verkeerslicht::switchVerkeerslichten(Verkeerslicht* vk1, Verkeerslicht* vk2)
{
    assert(vk1 != nullptr);
    assert(vk2 != nullptr);

    bool oude_waarde_vk1 = vk1->isGroen();
    bool oude_waarde_vk2 = vk2->isGroen();

    vk1->switchColor();
    vk1->tijd_sinds_laatste_verandering = 0;

    vk2->switchColor();
    vk2->tijd_sinds_laatste_verandering = 0;

    assert(vk1->isGroen() != oude_waarde_vk1);
    assert(vk2->isGroen() != oude_waarde_vk2);
}


void Verkeerslicht::updateVerkeerslicht()
{
    assert(properlyInit());
    assert(baan != nullptr);

    double oude_tijd = tijd_sinds_laatste_verandering;

    //2 situaties: 1. verkeerslicht staat op een kruisputn/2. niet op een kruispunt
    if(opKruispunt) {
        Verkeerslicht* verkeerslicht2 = (opKruispuntPair.first == this) ? opKruispuntPair.second : opKruispuntPair.first;
        // Als verkeerslicht 1 en 2 allebei een voertuig in hun stopzone hebben of net niet in hun stopzone hebben dan volg gwn normale cyclus
        if(VoertuigInVertraagzone == verkeerslicht2->VoertuigInVertraagzone)
        {
            // volg normale cyclus
            if (tijd_sinds_laatste_verandering > cyclus)
            {
                switchColor();
                tijd_sinds_laatste_verandering = 0;
            }
        }
        // Als er een voertuig voor licht 1 is en niet voor licht 2 switch licht 1 dan naar groen na 10 seconden
        if(VoertuigInVertraagzone && !verkeerslicht2->VoertuigInVertraagzone)
        {
            if ((tijd_sinds_laatste_verandering > 10) && !isGroen())
            {
                switchVerkeerslichten(this, verkeerslicht2);
            }
        }
        // Als een licht langer als 6 seconden op dezelfde kleur is verander zowizo
        if(tijd_sinds_laatste_verandering>60)
            switchVerkeerslichten(this,verkeerslicht2);
    }

    else // volg gewoon normale cyclus
    {
        if (tijd_sinds_laatste_verandering > cyclus)
        {
            switchColor();
            tijd_sinds_laatste_verandering = 0;
        }
    }

    tijd_sinds_laatste_verandering +=SIMULATIE_TIJD;

    // Update de voertuigen adhv de lichten
    auto voertuigen = baan->getVoertuigen();

    // Check of er een Voertuig in de vertraag zone is
    VoertuigInVertraagzone = false;
    for (const auto& v : voertuigen)
    {
        double afstand = positie - v->getPositie();
        if (afstand > 0 && afstand <= 50)
        {
            VoertuigInVertraagzone = true;
            break; // Geen verdere checks nodig
        }
    }

    // Als het groen is laat de autos dan versnellen
    if (groen)
    {
        for (const auto v : voertuigen)
        {
            if (positie - v->getPositie() > 0 && positie - v->getPositie() <= 50)
                v->accelerate();


        }
    }
        // anders laten we het eerste voertuig vertragen zodat alle autos stoppen
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

        // If het eerste voertuig voor het verkeerslicht is een prioriteitsvoertuig
        if (eerst_voertuig->isPrioriteitsVoertuig())
        {
            // ======= THEN voertuig hoeft niet te vertragen of te stoppen =======
            return;
        }

        // Laat het voertuig vertragen als het in de vertraag afstand is
        if (afstand_van_licht > STOP_AFSTAND && afstand_van_licht < VERTRAAG_AFSTAND && eerst_voertuig->getState() != State::SLOWINGDOWN && eerst_voertuig->getState() != State::STOPPING)
            eerst_voertuig->slowDown();

            // Laat het voertuig stoppen als het in de stop afstand is
        else if (afstand_van_licht < 1) // afstand_van_licht < STOP_AFSTAND
            eerst_voertuig->stop();
    }

    // Post-condities
    assert(tijd_sinds_laatste_verandering > oude_tijd || tijd_sinds_laatste_verandering == 0.166 || tijd_sinds_laatste_verandering == 0.0166 );
}

void Verkeerslicht::OpKruisPunt() {
    assert(properlyInit());
    this->opKruispunt = true;
    assert(isOpKruispunt() == true);
}

void Verkeerslicht::KruispuntPair(Verkeerslicht* v1, Verkeerslicht* v2) {
    assert(properlyInit());
    assert(v1 != nullptr);
    assert(v2 != nullptr);
    this->opKruispuntPair = {v1,v2};
}

void Verkeerslicht::setOranjeCyclus(double c) {
    assert(properlyInit());
    assert(c >= 0);
    oranjecyclus.first = c;
    oranjecyclus.second = 0;
}

bool Verkeerslicht::isOpKruispunt() const {
    assert(properlyInit());
    return opKruispunt;
}

void Verkeerslicht::setOpKruispunt(bool OpKruispunt) {
    assert(properlyInit());
    Verkeerslicht::opKruispunt = OpKruispunt;
    assert(isOpKruispunt() == OpKruispunt);
}

bool Verkeerslicht::isOranje() const {
    assert(properlyInit());
    return oranje;
}

void Verkeerslicht::setOranje(bool Oranje) {
    assert(properlyInit());
    Verkeerslicht::oranje = Oranje;
    assert(isOranje() == Oranje);
}