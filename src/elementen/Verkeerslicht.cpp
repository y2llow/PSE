//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#include "Verkeerslicht.h"

#include <cmath>
#include <iostream>

#include "Constants.h"
#include "../simulatie/Simulator.h"
#include "../DesignByContract.h"


Baan* Verkeerslicht::getBaan() const
{
    REQUIRE(properlyInit(), "Verkeerslicht is niet correct geïnitialiseerd");
    return baan;
}


void Verkeerslicht::setPositie(const double positie)
{
    REQUIRE(properlyInit(), "Verkeerslicht is niet correct geïnitialiseerd");
    this->positie = positie;
    ENSURE(getPositie() == positie, "Positie is niet correct ingesteld");
}

double Verkeerslicht::getCyclus() const
{
    REQUIRE(properlyInit(), "Verkeerslicht is niet correct geïnitialiseerd");
    return cyclus;
}

void Verkeerslicht::setCyclus(const double cyclus)
{
    REQUIRE(properlyInit(), "Verkeerslicht is niet correct geïnitialiseerd");
    REQUIRE(cyclus > 0, "Cyclus moet groter zijn dan 0");
    this->cyclus = cyclus;
    ENSURE(getCyclus() == cyclus, "Cyclus is niet correct ingesteld");
}

void Verkeerslicht::setBaan(Baan* baan)
{
    REQUIRE(properlyInit(), "Verkeerslicht is niet correct geïnitialiseerd");
    this->baan = baan;
    ENSURE(getBaan() == baan, "Baan is niet correct ingesteld");
}

double Verkeerslicht::getPositie() const
{
    REQUIRE(properlyInit(), "Verkeerslicht is niet correct geïnitialiseerd");
    return positie;
}

bool Verkeerslicht::isGroen() const
{
    REQUIRE(properlyInit(), "Verkeerslicht is niet correct geïnitialiseerd");
    return groen;
}

void Verkeerslicht::switchColor()
{
    REQUIRE(properlyInit(), "Verkeerslicht is niet correct geïnitialiseerd");
    bool oude_waarde = groen;
    groen = !groen;
    ENSURE(isGroen() != oude_waarde, "Kleur is niet correct omgeschakeld");
}

void Verkeerslicht::switchVerkeerslichten(Verkeerslicht* vk1, Verkeerslicht* vk2)
{
    REQUIRE(vk1 != nullptr, "Eerste verkeerslicht mag niet nullptr zijn");
    REQUIRE(vk2 != nullptr, "Tweede verkeerslicht mag niet nullptr zijn");

    bool oude_waarde_vk1 = vk1->isGroen();
    bool oude_waarde_vk2 = vk2->isGroen();

    vk1->switchColor();
    vk1->tijd_sinds_laatste_verandering = 0;

    vk2->switchColor();
    vk2->tijd_sinds_laatste_verandering = 0;

    ENSURE(vk1->isGroen() != oude_waarde_vk1, "Kleur van eerste verkeerslicht is niet correct omgeschakeld");
    ENSURE(vk2->isGroen() != oude_waarde_vk2, "Kleur van tweede verkeerslicht is niet correct omgeschakeld");
}


void Verkeerslicht::updateVerkeerslicht()
{
    REQUIRE(properlyInit(), "Verkeerslicht is niet correct geïnitialiseerd");
    REQUIRE(baan != nullptr, "Baan mag niet nullptr zijn");

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

    tijd_sinds_laatste_verandering += SIMULATIE_TIJD;

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
        if (afstand_van_licht > STOP_AFSTAND && afstand_van_licht < VERTRAAG_AFSTAND &&
            eerst_voertuig->getState() != State::SLOWINGDOWN && eerst_voertuig->getState() != State::STOPPING)
            eerst_voertuig->slowDown();

            // Laat het voertuig stoppen als het in de stop afstand is
        else if (afstand_van_licht < 1) // afstand_van_licht < STOP_AFSTAND
            eerst_voertuig->stop();
    }

    // Post-condities
    ENSURE(tijd_sinds_laatste_verandering > oude_tijd || tijd_sinds_laatste_verandering == 0.166 ||
           tijd_sinds_laatste_verandering == 0.0166, "Tijd sinds laatste verandering is niet correct bijgewerkt");
}

void Verkeerslicht::OpKruisPunt() {
    REQUIRE(properlyInit(), "Verkeerslicht is niet correct geïnitialiseerd");
    this->opKruispunt = true;
    ENSURE(isOpKruispunt() == true, "Verkeerslicht is niet correct gemarkeerd als zijnde op kruispunt");
}

void Verkeerslicht::KruispuntPair(Verkeerslicht* v1, Verkeerslicht* v2) {
    REQUIRE(properlyInit(), "Verkeerslicht is niet correct geïnitialiseerd");
    REQUIRE(v1 != nullptr, "Eerste verkeerslicht mag niet nullptr zijn");
    REQUIRE(v2 != nullptr, "Tweede verkeerslicht mag niet nullptr zijn");
    this->opKruispuntPair = {v1,v2};
}

void Verkeerslicht::setOranjeCyclus(double c) {
    REQUIRE(properlyInit(), "Verkeerslicht is niet correct geïnitialiseerd");
    REQUIRE(c >= 0, "Oranje cyclus moet groter dan of gelijk aan 0 zijn");
    oranjecyclus.first = c;
    oranjecyclus.second = 0;
}

bool Verkeerslicht::isOpKruispunt() const {
    REQUIRE(properlyInit(), "Verkeerslicht is niet correct geïnitialiseerd");
    return opKruispunt;
}

void Verkeerslicht::setOpKruispunt(bool OpKruispunt) {
    REQUIRE(properlyInit(), "Verkeerslicht is niet correct geïnitialiseerd");
    Verkeerslicht::opKruispunt = OpKruispunt;
    ENSURE(isOpKruispunt() == OpKruispunt, "OpKruispunt is niet correct ingesteld");
}

bool Verkeerslicht::isOranje() const {
    REQUIRE(properlyInit(), "Verkeerslicht is niet correct geïnitialiseerd");
    return oranje;
}

void Verkeerslicht::setOranje(bool Oranje) {
    REQUIRE(properlyInit(), "Verkeerslicht is niet correct geïnitialiseerd");
    Verkeerslicht::oranje = Oranje;
    ENSURE(isOranje() == Oranje, "Oranje is niet correct ingesteld");
}