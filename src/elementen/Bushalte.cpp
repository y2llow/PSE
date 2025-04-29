//
// Created by s0243673 on 24/04/2025.
//

#include "Bushalte.h"

#include "Constants.h"
#include "voertuigen/Bus.h"


Baan* Bushalte::getBaan() const
{
    return baan;
}

void Bushalte::setBaan(Baan* baan)
{
    this->baan = baan;
}

void Bushalte::setPositie(const double positie)
{
    this->positie = positie;
}

double Bushalte::getPositie() const
{
    return positie;
}

double Bushalte::getWachttijd() const
{
    return wachttijd;
}

void Bushalte::setWachttijd(const double wt)
{
    this->wachttijd = wt;
}

void Bushalte::stopBus() const
{
    Bus* bus = nullptr;
    int distance = 0;

    for (const auto voertuig : baan->getVoertuigen())
    {
        if (voertuig->getType() == "Bus")
        {
            distance = positie - voertuig->getPositie();
            if (distance < 0 || distance > VERTRAAG_AFSTAND)
                continue;

            bus = dynamic_cast<Bus*>(voertuig);
            break;
        }
    }

    if (bus == nullptr)
        return;

    const double time = bus->getTimeSindsStopped();

    if (time > wachttijd)
    {
        bus->accelerate();
        bus->setTimeSindsStopped(0);
        return;
    }

    if (distance > 0 && distance < STOP_AFSTAND)
    {
        bus->setState(State::STOPPING);

        bus->setTimeSindsStopped(time + SIMULATIE_TIJD);

    }
    else if (distance > 0 && distance < VERTRAAG_AFSTAND && !bus->isSlowed())
        bus->slowDown();
}
