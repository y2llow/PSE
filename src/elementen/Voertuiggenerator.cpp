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

        for (const auto v : baan->getVoertuigen())
        {
            if (v->getPositie() >= 0 && v->getPositie() <= 2)
            {
                geen_voertuig = false;
                break;
            }
        }
        tijd_sinds_laatste_voertuig = 0;

        if (geen_voertuig)
        {
            const auto voertuig = new Auto();
            baan->addVoertuig(voertuig);
            voertuig->setBaan(baan);
            voertuig->setPositie(0);
        }
    }


    tijd_sinds_laatste_voertuig += SIMULATIE_TIJD;
}
