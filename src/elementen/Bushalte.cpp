//
// Created by s0243673 on 24/04/2025.
//

#include "Bushalte.h"

#include <iostream>
#include <algorithm>

#include "Constants.h"
#include "../simulatie/Simulator.h"
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

void Bushalte::stopBus()
{
    Bus* bus = nullptr;
    double distance = 0;

    for (const auto voertuig : baan->getVoertuigen())
    {
        if (voertuig->getType() == "Bus")
        {
            distance = positie - voertuig->getPositie();

            if (distance < 0 || distance > VERTRAAG_AFSTAND || find(waited_busses.begin(), waited_busses.end(),
                                                                    voertuig) != waited_busses.end())
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
        waited_busses.push_back(bus);
        return;
    }

    if (bus->getState() == State::STOPPING)
    {
        bus->setTimeSindsStopped(time + SIMULATIE_TIJD);
        return;
    }


    // Normaal gezien moet de bus beginnen stop als hij in de stopafstand is, maar dan stopt hij 15 m voor de bushalte
    // Daar heb ik hier distance == 0 i.p.v distance > 0 && distance < STOP_AFSTAND
    if (distance < 0.5)
    {
        bus->stop();
        Simulator::printStatus(bus);
        cout << "Time: " << time << endl << endl;
    }
    else if (distance > 0 && distance < VERTRAAG_AFSTAND && bus->getState() != State::SLOWINGDOWN)
        bus->slowDown();
}
