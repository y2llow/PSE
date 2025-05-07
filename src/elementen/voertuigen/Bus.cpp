//
// Created by Abdellah on 4/28/2025.
//

#include "Bus.h"
#include <cassert>

[[nodiscard]] double Bus::getTimeSindsStopped() const
{
    assert(properlyInit());
    return tijd_sinds_stopped;
}

void Bus::setTimeSindsStopped(double t)
{
    assert(properlyInit());
    tijd_sinds_stopped = t;
    assert(getTimeSindsStopped() == t);
}

string Bus::getType() const
{
    assert(properlyInit());
    return "Bus";
}