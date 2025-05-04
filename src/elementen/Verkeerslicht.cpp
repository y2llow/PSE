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


    //als de tijd sinds laatste verandering 60 seconden is, veranderd de verkeerslichts kleur
    if (tijd_sinds_laatste_verandering >= 60){
        switchColor();
        tijd_sinds_laatste_verandering = 0;
    }
    //of als de tijd sinds laatste verandering tussen 10 en 60 seconden is
    else if (tijd_sinds_laatste_verandering >= 10 && tijd_sinds_laatste_verandering < 60){
        //als verkeerslicht rood is
        if (!groen){
           //als voertuig binnen de 50 meter van het verkeerslicht is
           for (const auto v : baan->getVoertuigen()){
               //licht groen maken, tijd sinds laatste verandering =0
               if (positie - v->getPositie() > 0 && positie - v->getPositie() <= 50){
                   switchColor();
                   tijd_sinds_laatste_verandering = 0;

                   //kijken als dit op een kruispunt staat
                   if(this->verkeerslichtOpKruispunt){
                       this->verkeerlichtenOpKruispuntPair.second->switchColor();
                       this->verkeerlichtenOpKruispuntPair.second->tijd_sinds_laatste_verandering = 0;
                   }

                   break;
               }
           }
        }
    }


//    if (tijd_sinds_laatste_verandering > cyclus)
//    {
//        switchColor();
//        tijd_sinds_laatste_verandering = 0;
//    }

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

void Verkeerslicht::OpKruisPunt() {
    this->verkeerslichtOpKruispunt = true;
}

void Verkeerslicht::KruispuntPair(Verkeerslicht* v1, Verkeerslicht* v2) {
    this->verkeerlichtenOpKruispuntPair  = {v1,v2};
}
