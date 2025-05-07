#include "Bushalte.h"

#include <iostream>
#include <algorithm>
#include <cassert>

#include "Constants.h"
#include "../simulatie/Simulator.h"
#include "voertuigen/Bus.h"

Baan* Bushalte::getBaan() const {
    assert(properlyInit());
    return baan;
}

void Bushalte::setBaan(Baan* baan) {
    assert(properlyInit());
    assert(baan != nullptr);
    this->baan = baan;
    assert(getBaan() == baan);
}

double Bushalte::getPositie() const {
    assert(properlyInit());
    return positie;
}

void Bushalte::setPositie(const double positie) {
    assert(properlyInit());
    assert(positie >= 0);
    this->positie = positie;
    assert(getPositie() == positie);
}

double Bushalte::getWachttijd() const {
    assert(properlyInit());
    return wachttijd;
}

void Bushalte::setWachttijd(const double wt) {
    assert(properlyInit());
    assert(wt >= 0);
    this->wachttijd = wt;
    assert(getWachttijd() == wt);
}

void Bushalte::stopBus() {
    assert(properlyInit());
    assert(baan != nullptr);

    Bus* bus = nullptr;
    double distance = 0;

    for (const auto voertuig : baan->getVoertuigen()) {
        if (voertuig->getType() == "Bus") {
            distance = positie - voertuig->getPositie();

            if (distance < 0 || distance > VERTRAAG_AFSTAND || find(waited_busses.begin(), waited_busses.end(), voertuig) != waited_busses.end())
                continue;

            bus = dynamic_cast<Bus*>(voertuig);
            break;
        }
    }

    if (bus == nullptr)
        return;

    double time = bus->getTimeSindsStopped();

    if (time > wachttijd)
    {
        bus->accelerate();
        bus->setTimeSindsStopped(0);
        waited_busses.push_back(bus);
        return;
    }

    // if (bus->getState() == State::STOPPING)
    // {
    //     bus->setTimeSindsStopped(time + SIMULATIE_TIJD);
    //     return;
    // }

    // Normaal gezien moet de bus beginnen stop als hij in de stopafstand is, maar dan stopt hij 15 m voor de bushalte
    // Daar heb ik hier distance == 0 i.p.v distance > 0 && distance < STOP_AFSTAND
    if (distance < 0.5)
    {
        bus->stop();
        bus->setTimeSindsStopped(time + SIMULATIE_TIJD);
    } else if (distance > 0 && distance < VERTRAAG_AFSTAND &&
               bus->getState() != State::SLOWINGDOWN &&
               bus->getState() != State::STOPPING) {
        bus->slowDown();
    }

    // Postconditie check:
    // Dezelfde bus kan max 1 keer in waited_busses staan.
    assert(std::count(waited_busses.begin(), waited_busses.end(), bus) <= 1);
}
