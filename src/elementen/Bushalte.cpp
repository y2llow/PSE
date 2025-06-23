#include "Bushalte.h"
#include "../DesignByContract.h"

#include <iostream>
#include <algorithm>

#include "Constants.h"
#include "../simulatie/Simulator.h"
#include "voertuigen/Bus.h"

Baan* Bushalte::getBaan() const {
    REQUIRE(properlyInit(), "Bushalte is niet correct geïnitialiseerd");
    return baan;
}

void Bushalte::setBaan(Baan* baan) {
    REQUIRE(properlyInit(), "Bushalte is niet correct geïnitialiseerd");
    REQUIRE(baan != nullptr, "Baan mag niet nullptr zijn");
    this->baan = baan;
    ENSURE(getBaan() == baan, "Baan is niet correct ingesteld");
}

double Bushalte::getPositie() const {
    REQUIRE(properlyInit(), "Bushalte is niet correct geïnitialiseerd");
    return positie;
}

void Bushalte::setPositie(const double positie) {
    REQUIRE(properlyInit(), "Bushalte is niet correct geïnitialiseerd");
    REQUIRE(positie >= 0, "Positie moet groter dan of gelijk aan 0 zijn");
    this->positie = positie;
    ENSURE(getPositie() == positie, "Positie is niet correct ingesteld");
}

double Bushalte::getWachttijd() const {
    REQUIRE(properlyInit(), "Bushalte is niet correct geïnitialiseerd");
    return wachttijd;
}

void Bushalte::setWachttijd(const double wt) {
    REQUIRE(properlyInit(), "Bushalte is niet correct geïnitialiseerd");
    REQUIRE(wt >= 0, "Wachttijd moet groter dan of gelijk aan 0 zijn");
    this->wachttijd = wt;
    ENSURE(getWachttijd() == wt, "Wachttijd is niet correct ingesteld");
}

void Bushalte::stopBus() {
    REQUIRE(properlyInit(), "Bushalte is niet correct geïnitialiseerd");
    REQUIRE(baan != nullptr, "Baan mag niet nullptr zijn");

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
    ENSURE(std::count(waited_busses.begin(), waited_busses.end(), bus) <= 1,
           "Bus is meer dan één keer toegevoegd aan waited_busses");
}