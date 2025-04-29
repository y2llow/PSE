//
// Created by Abdellah on 4/28/2025.
//

#include "Bus.h"

string Bus::getType() const
{
    return "Bus";
}


double Bus::getTimeSindsStopped() const
{
    return time_sinds_stopped;
}

void Bus::setTimeSindsStopped(const double time_sinds_stopped)
{
    this->time_sinds_stopped = time_sinds_stopped;
}


