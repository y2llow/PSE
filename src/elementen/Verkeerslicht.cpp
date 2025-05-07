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

void Verkeerslicht::switchVerkeerslichten(Verkeerslicht* vk1, Verkeerslicht* vk2)
{
    vk1->switchColor();
    vk1->tijd_sinds_laatste_verandering = 0;

    vk2->switchColor();
    vk2->tijd_sinds_laatste_verandering = 0;
}


void Verkeerslicht::updateVerkeerslicht()
{
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



        //op een kruispunt

        //3 situaties:
            //1. tijd sinds laatste verandering is is groter dan 60 min de volldeige oranje cyclus
            //2. de tijd staat tussen 10 en 60 min de volledige oranje cyclus
            //3. de tijd is onder de 10 seconden

        //1.
    //     if (tijd_sinds_laatste_verandering>= 60 - oranjecyclus.first){
    //         //3 situaties:
    //             //groen
    //             //oranje
    //             //rood
    //
    //             //groen
    //             if(groen){
    //                 oranjecyclus.second+=SIMULATIE_TIJD;
    //                 tijd_sinds_laatste_verandering+=SIMULATIE_TIJD;
    //                 //kleur moet veranderen, oranje wordt eerst verwerkt, oranjecyclus wordt vermeerdert.
    //                 switchColor();
    //                 oranje = true;
    //             }
    //
    //             //oranje
    //             else if (!groen && oranje){
    //                 //de oranjeCyclus tijd vermeerderen. kijken als dit cyclus klaar is
    //                 oranjecyclus.second+=SIMULATIE_TIJD;
    //                 tijd_sinds_laatste_verandering +=SIMULATIE_TIJD;
    //
    //                 //als cyclus klaar is (de oranjes van tegenovergestelde verkeerslichten beinvloeden de cyclus bij zowel die ene als de andere verkeerslicht)
    //
    //                 //bijvoorbeeld:
    //                     //VL1: groen->rood: de functie hieronder zorgt ervoor dat de rode licht van de andere verkeerslicht groen wordt, wat de cyclus restart.
    //                     //dus wanneer VL1 groen->rood ondergaat: VL2 rood->groen ondergaat.
    //                 if (oranjecyclus.second>=oranjecyclus.first){
    //                     //het is rood ->tegenovergestelde verkeerslicht mag groen worden, simulatietijd sinds laatste verandering en oranjeCylcus op 0 zetten
    //                     oranje = false;
    //
    //                     opKruispuntPair.second->switchColor();
    //
    //                     opKruispuntPair.second->tijd_sinds_laatste_verandering = 0;
    //
    //                     oranjecyclus.second = 0;
    //
    //                     tijd_sinds_laatste_verandering = 0;
    //                 }
    //             }
    //     }
    //
    //     //2.
    //     else if (tijd_sinds_laatste_verandering>=10 && tijd_sinds_laatste_verandering<=60-oranjecyclus.first) {
    //         //3 situaties:
    //         //groen
    //         // oranje
    //         //rood
    //
    //         //groen
    //         if (groen && !oranje) {
    //             tijd_sinds_laatste_verandering += SIMULATIE_TIJD;
    //             //als er geen voertuigen zijn in de volgende 300 meter -> rood, anders niks veranderen. : tegenover gestelde verkeerslicht mag groen worden pas als dit volledig rood wordt
    //             for (const auto v: baan->getVoertuigen()) {
    //                 if (positie - v->getPositie() >= 300) {
    //                     oranjecyclus.second += SIMULATIE_TIJD;
    //                     switchColor();
    //                     oranje = true;
    //                     break;
    //                 }
    //             }
    //         }
    //
    //             //oranje
    //         else if (!groen && oranje) {
    //             //als de tijd van de oranje cyclus groot genoeg is dan maken we de licht rood -> tegenover gestelde licht mag hierna pas groen worden
    //             tijd_sinds_laatste_verandering += SIMULATIE_TIJD;
    //
    //             if (oranjecyclus.second >= oranjecyclus.first) {
    //                 oranjecyclus.second = 0;
    //
    //                 oranje = false;
    //
    //                 tijd_sinds_laatste_verandering = 0;
    //
    //                 //andere verkeerslicht wordt groen
    //                 opKruispuntPair.second->switchColor();
    //             }
    //         }
    //
    //             //rood
    //         else if (!groen && !oranje) {
    //             tijd_sinds_laatste_verandering +=SIMULATIE_TIJD;
    //             //als er een voertuig is in de volgende 50 meter dan naar groen omzetten als kan
    //             for (const auto v: baan->getVoertuigen()) {
    //                 if (positie - v->getPositie() <= 50) {
    //                     opKruispuntPair.second->oranjecyclus.second+=SIMULATIE_TIJD;
    //                     opKruispuntPair.second->oranje = true;
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    //
    //     //3.
    //     else{
    //         tijd_sinds_laatste_verandering +=SIMULATIE_TIJD;
    //     }
    // }

    else // volg gewoon normale cyclus
    {
        if (tijd_sinds_laatste_verandering > cyclus)
        {
            switchColor();
            tijd_sinds_laatste_verandering = 0;
        }
    }

    //     if(tijd_sinds_laatste_verandering>= 60 - oranjecyclus.first){
    //         if (groen){
    //             oranjecyclus.second+=SIMULATIE_TIJD;
    //             tijd_sinds_laatste_verandering +=SIMULATIE_TIJD;
    //
    //             oranje = true;
    //
    //             switchColor();
    //         }
    //         else if (!groen && oranje){
    //             oranjecyclus.second+=SIMULATIE_TIJD;
    //             tijd_sinds_laatste_verandering += SIMULATIE_TIJD;
    //
    //             if (oranjecyclus.second >=oranjecyclus.first){
    //                 oranjecyclus.second = 0;
    //                 tijd_sinds_laatste_verandering = 0;
    //                 oranje = false;
    //             }
    //         }
    //         else if(!groen && ! oranje){
    //             switchColor();
    //             tijd_sinds_laatste_verandering =0;
    //         }
    //     }
    //     else if(tijd_sinds_laatste_verandering>=10 && tijd_sinds_laatste_verandering<=60-oranjecyclus.first){
    //         if (groen){
    //             tijd_sinds_laatste_verandering +=SIMULATIE_TIJD;
    //
    //             for (auto const v: baan->getVoertuigen()){
    //                 if(positie - v->getPositie() <=300){
    //                     switchColor();
    //                     oranje = true;
    //                     oranjecyclus.second+=SIMULATIE_TIJD;
    //                 }
    //             }
    //         }
    //         else if(!groen && oranje){
    //             tijd_sinds_laatste_verandering +=SIMULATIE_TIJD;
    //
    //             oranjecyclus.second+=SIMULATIE_TIJD;
    //
    //             if(oranjecyclus.second>=oranjecyclus.first){
    //                 oranje = false;
    //                 tijd_sinds_laatste_verandering = 0;
    //                 oranjecyclus.second = 0;
    //             }
    //         }
    //         else if(!groen && !oranje){
    //             tijd_sinds_laatste_verandering += SIMULATIE_TIJD;
    //
    //             for (auto const v: baan->getVoertuigen()){
    //                 if (positie - v->getPositie()<=50){
    //                     switchColor();
    //                     tijd_sinds_laatste_verandering = 0;
    //                 }
    //             }
    //         }
    //         else{
    //             tijd_sinds_laatste_verandering +=SIMULATIE_TIJD;
    //         }
    //     }
    // }

//    //als de tijd sinds laatste verandering 60 seconden is, veranderd de verkeerslichts kleur
//    if (tijd_sinds_laatste_verandering >= 60){
//        //als de verkeerslicht groen is
//        if(groen){
//            tijd_sinds_laatste_verandering += SIMULATIE_TIJD;
//            switchColor();
//            oranje = true;
//
//            if(verkeerslichtOpKruispunt){
//                verkeerlichtenOpKruispuntPair.second->switchColor();
//            }
//        }
//
//        if (!groen && oranje){
//            oranjecyclus.second+=SIMULATIE_TIJD;
//
//            if(oranjecyclus.second>=oranjecyclus.first){
//                oranjecyclus.second = 0;
//                oranje = false;
//            }
//
//        }
//
//        if(!groen && !oranje){
//            switchColor();
//            tijd_sinds_laatste_verandering = 0;
//
//        }
//    }
//    //of als de tijd sinds laatste verandering tussen 10 en 60 seconden is
//    else if (tijd_sinds_laatste_verandering >= 10 && tijd_sinds_laatste_verandering < 60){
//        //als verkeerslicht rood is
//        if (!groen && !oranje){
//           //als voertuig binnen de 50 meter van het verkeerslicht is
//           for (const auto v : baan->getVoertuigen()){
//               //licht groen maken, tijd sinds laatste verandering =0
//               if (positie - v->getPositie() > 0 && positie - v->getPositie() <= 50){
//                   switchColor();
//                   tijd_sinds_laatste_verandering = 0;
//
//                   //kijken als dit op een kruispunt staat
//                   if(verkeerslichtOpKruispunt){
//                       verkeerlichtenOpKruispuntPair.second->switchColor();
//                       verkeerlichtenOpKruispuntPair.second->oranje = true;
//                       verkeerlichtenOpKruispuntPair.second->tijd_sinds_laatste_verandering = 60;
//                   }
//                   break;
//               }
//           }
//        }
//    }


//    if (tijd_sinds_laatste_verandering > cyclus)
//    {
//        switchColor();
//        tijd_sinds_laatste_verandering = 0;
//    }

    // assert(baan != nullptr);

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
}

void Verkeerslicht::OpKruisPunt() {
    this->opKruispunt = true;
}

void Verkeerslicht::KruispuntPair(Verkeerslicht* v1, Verkeerslicht* v2) {
    this->opKruispuntPair  = {v1,v2};
}

void Verkeerslicht::setOranjeCyclus(double c) {
    oranjecyclus.first = c;
    oranjecyclus.second = 0;
}

bool Verkeerslicht::isOpKruispunt() const {
    return opKruispunt;
}

void Verkeerslicht::setOpKruispunt(bool OpKruispunt) {
    Verkeerslicht::opKruispunt = OpKruispunt;
}

bool Verkeerslicht::isOranje() const {
    return oranje;
}

void Verkeerslicht::setOranje(bool Oranje) {
    Verkeerslicht::oranje = Oranje;
}
