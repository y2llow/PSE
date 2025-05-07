//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#include "Voertuiggenerator.h"
#include "../DesignByContract.h"

#include "Constants.h"
#include "Voertuig.h"
#include "voertuigen/Auto.h"


Baan* Voertuiggenerator::getBaan() const
{
    REQUIRE(properlyInit(), "Voertuiggenerator is niet correct geïnitialiseerd");
    return baan;
}

void Voertuiggenerator::setBaan(Baan* b)
{
    REQUIRE(properlyInit(), "Voertuiggenerator is niet correct geïnitialiseerd");
    this->baan = b;
    ENSURE(getBaan() == b, "Baan is niet correct ingesteld");
}

double Voertuiggenerator::getFrequentie() const
{
    REQUIRE(properlyInit(), "Voertuiggenerator is niet correct geïnitialiseerd");
    return frequentie;
}

void Voertuiggenerator::setFrequentie(const double frequentie)
{
    REQUIRE(properlyInit(), "Voertuiggenerator is niet correct geïnitialiseerd");
    REQUIRE(frequentie >= 0, "Frequentie moet groter dan of gelijk aan 0 zijn");
    this->frequentie = frequentie;
    ENSURE(getFrequentie() == frequentie, "Frequentie is niet correct ingesteld");
}

string Voertuiggenerator::getType()
{
    REQUIRE(properlyInit(), "Voertuiggenerator is niet correct geïnitialiseerd");
    return type;
}

void Voertuiggenerator::generateVoertuig()
{
    REQUIRE(properlyInit(), "Voertuiggenerator is niet correct geïnitialiseerd");
    REQUIRE(baan != nullptr, "Baan mag niet nullptr zijn");

    double oude_tijd = tijd_sinds_laatste_voertuig;

    if (tijd_sinds_laatste_voertuig > frequentie)
    {
        bool geen_voertuig = true;
        Voertuig* temp_voertuig_voor_lengte = Voertuig::createVoertuig(type);
        for (const auto v : baan->getVoertuigen())
        {
            if (v->getPositie() >= 0 && v->getPositie() <= 2 * temp_voertuig_voor_lengte->getLengte())
            {
                geen_voertuig = false;
                break;
            }
        }

        if (geen_voertuig)
        {
            const auto voertuig = Voertuig::createVoertuig(type);
            baan->addVoertuig(voertuig);
            voertuig->setBaan(baan);

            tijd_sinds_laatste_voertuig = 0;
            ENSURE(tijd_sinds_laatste_voertuig == 0, "Tijd sinds laatste voertuig is niet gereset");
        }

        delete temp_voertuig_voor_lengte;
    }

    tijd_sinds_laatste_voertuig += SIMULATIE_TIJD;

    // Als geen voertuig is toegevoegd, moet de tijd verhoogd zijn
    if (oude_tijd == tijd_sinds_laatste_voertuig - SIMULATIE_TIJD) {
        ENSURE(tijd_sinds_laatste_voertuig == oude_tijd + SIMULATIE_TIJD,
               "Tijd sinds laatste voertuig is niet correct bijgewerkt");
    }
}