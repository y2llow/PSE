//
// Created by Abdellah on 4/28/2025.
//

#include "Bus.h"
#include "../../DesignByContract.h"

[[nodiscard]] double Bus::getTimeSindsStopped() const
{
    REQUIRE(properlyInit(), "Bus is niet correct geïnitialiseerd");
    return tijd_sinds_stopped;
}

void Bus::setTimeSindsStopped(double t)
{
    REQUIRE(properlyInit(), "Bus is niet correct geïnitialiseerd");
    tijd_sinds_stopped = t;
    ENSURE(getTimeSindsStopped() == t, "Tijd sinds gestopt is niet correct ingesteld");
}

string Bus::getType() const
{
    REQUIRE(properlyInit(), "Bus is niet correct geïnitialiseerd");
    return "Bus";
}