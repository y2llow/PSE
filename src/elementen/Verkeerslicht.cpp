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
    return baan;
}


void Verkeerslicht::setPositie(const double positie)
{
    this->positie = positie;
}

double Verkeerslicht::getCyclus() const
{
    return cyclus;
}

void Verkeerslicht::setCyclus(const double cyclus)
{
    this->cyclus = cyclus;
}

void Verkeerslicht::setBaan(Baan* baan)
{
    this->baan = baan;
}

double Verkeerslicht::getPositie() const
{
    return positie;
}

bool Verkeerslicht::isGroen() const
{
    return groen;
}

void Verkeerslicht::switchColor()
{
    groen = !groen;
}

void Verkeerslicht::updateVerkeerslicht()
{
    tijd_sinds_laatste_verandering += SIMULATIE_TIJD;

    VerkeerslichtenOpKruispunten();

    if (tijd_sinds_laatste_verandering > cyclus)
    {
        switchColor();
        tijd_sinds_laatste_verandering = 0;
    }

    // assert(baan != nullptr);

    auto voertuigen = baan->getVoertuigen();

    if (groen)
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
            // eerst_voertuig->accelerate();
            return;
        }

        if (afstand_van_licht > STOP_AFSTAND && afstand_van_licht < VERTRAAG_AFSTAND && eerst_voertuig->getState() != State::SLOWINGDOWN)
            eerst_voertuig->slowDown();
        else if (afstand_van_licht < 1) // afstand_van_licht < STOP_AFSTAND
        {
            eerst_voertuig->stop();
        }
    }
}

void Verkeerslicht::VerkeerslichtenOpKruispunten() {
    Baan *currentbaan = getBaan();

    Verkeerslicht *verkeerslichtKP1 = nullptr;
    Verkeerslicht *verkeerslichtKP2 = nullptr;

    REQUIRE(!currentbaan->getVerkeerslichten().empty(), "geen verkeerslichten");
    REQUIRE(!currentbaan->kruispunten.empty(), "geen kruispunt");

    auto kruispunten = currentbaan->kruispunten;
//    for (auto &KP1: kruispunten) {
//        for (auto &KP2: KP1.second[0]->kruispunten) {
//            for (Verkeerslicht* licht : currentbaan->getVerkeerslichten()) {
//                for (Verkeerslicht *licht2: KP1.second[0]->getVerkeerslichten()) {
//                    double lichtPositie = licht->getPositie();
//                    double licht2Positie = licht2->getPositie();
//
//
//                    if (lichtPositie == KP1.first) { verkeerslichtKP1 = licht; }
//                    else if (lichtPositie == KP2.first) { verkeerslichtKP2 = licht; }
//                    if (verkeerslichtKP1 && verkeerslichtKP2) { break; }
//                }
//
//                if (verkeerslichtKP1 && verkeerslichtKP2 && verkeerslichtKP1 != verkeerslichtKP2) {
//                    VerkeerslichtenTegenstellen(verkeerslichtKP1, verkeerslichtKP2);
//                }
//            }
//        }
//    }

    for (auto &KP1: kruispunten) {
        // Zoek verkeerslicht op currentbaan dat matcht met KP1
        for (Verkeerslicht *licht: currentbaan->getVerkeerslichten()) {
            if ((licht->getPositie() == KP1.first)) {
                verkeerslichtKP1 = licht;
                break; // Stop na eerste match
            }
        }

        if (!verkeerslichtKP1) continue;

        for (auto &KP2: KP1.second[0]->kruispunten) {
            // Zoek verkeerslicht op de verbonden baan dat matcht met KP2
            for (Verkeerslicht *licht2: KP1.second[0]->getVerkeerslichten()) {
                if (licht2->getPositie() == KP2.first) {
                    verkeerslichtKP2 = licht2;
                    break; // Stop na eerste match
                }
            }

            // Als beide lichten gevonden zijn en verschillend zijn verander kleur en cyclus
            if (verkeerslichtKP2 != nullptr && verkeerslichtKP1 != verkeerslichtKP2) {
                VerkeerslichtenTegenstellen(verkeerslichtKP1, verkeerslichtKP2);
            }
        }
    }


//
//    for (auto &KP1: kruispunten) {
//        for (Verkeerslicht* licht : currentbaan->getVerkeerslichten()) {
//            double lichtPositie = licht->getPositie();
//
//            if(KP1.first == lichtPositie)
//            {
//                verkeerslichtKP1 = licht;
//            }
//        }
//    }
//    for (auto &KP2: KP1.second[0]->kruispunten) {
//        for (Verkeerslicht *licht2: KP1.second[0]->getVerkeerslichten()) {
//            double licht2Positie = licht2->getPositie();
//            if (licht2Positie == KP2.first) { verkeerslichtKP2 = licht2; }
//                }
//
//                if (verkeerslichtKP1 && verkeerslichtKP2 && verkeerslichtKP1 != verkeerslichtKP2) {
//                    VerkeerslichtenTegenstellen(verkeerslichtKP1, verkeerslichtKP2);
//                }
//            }
}

void Verkeerslicht::VerkeerslichtenTegenstellen(Verkeerslicht* VK1, Verkeerslicht* VK2)
{
    VK2->setCyclus(VK1->getCyclus());
    if (VK1->isGroen() == VK2->isGroen())
    {
        VK2->switchColor();
    }
}