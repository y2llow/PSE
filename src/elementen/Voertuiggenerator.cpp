//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#include "Voertuiggenerator.h"

#include "Constants.h"
#include "Voertuig.h"
#include "voertuigen/Auto.h"


Baan* Voertuiggenerator::getBaan() const
{
    return baan;
}

void Voertuiggenerator::setBaan(Baan* b)
{
    this->baan = b;
}

double Voertuiggenerator::getFrequentie() const
{
    return frequentie;
}

void Voertuiggenerator::setFrequentie(const double frequentie)
{
    this->frequentie = frequentie;
}

string Voertuiggenerator::getType()
{
    return type;
}

void Voertuiggenerator::generateVoertuig()
{
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
        }

        delete temp_voertuig_voor_lengte;
    }


    tijd_sinds_laatste_voertuig += SIMULATIE_TIJD;
}
