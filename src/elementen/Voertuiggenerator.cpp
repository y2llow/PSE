//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#include "Voertuiggenerator.h"
#include <cassert>

#include "Constants.h"
#include "Voertuig.h"
#include "voertuigen/Auto.h"


Baan* Voertuiggenerator::getBaan() const
{
    assert(properlyInit());
    return baan;
}

void Voertuiggenerator::setBaan(Baan* b)
{
    assert(properlyInit());
    this->baan = b;
    assert(getBaan() == b);
}

double Voertuiggenerator::getFrequentie() const
{
    assert(properlyInit());
    return frequentie;
}

void Voertuiggenerator::setFrequentie(const double frequentie)
{
    assert(properlyInit());
    assert(frequentie >= 0);
    this->frequentie = frequentie;
    assert(getFrequentie() == frequentie);
}

string Voertuiggenerator::getType()
{
    assert(properlyInit());
    return type;
}

void Voertuiggenerator::generateVoertuig()
{
    assert(properlyInit());
    assert(baan != nullptr);

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
            assert(tijd_sinds_laatste_voertuig == 0);
        }

        delete temp_voertuig_voor_lengte;
    }

    tijd_sinds_laatste_voertuig += SIMULATIE_TIJD;

    // Als geen voertuig is toegevoegd, moet de tijd verhoogd zijn
    if (oude_tijd == tijd_sinds_laatste_voertuig - SIMULATIE_TIJD) {
        assert(tijd_sinds_laatste_voertuig == oude_tijd + SIMULATIE_TIJD);
    }
}