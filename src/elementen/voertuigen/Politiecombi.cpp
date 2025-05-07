//
// Created by Abdellah on 4/28/2025.
//

#include "Politiecombi.h"
#include <cassert>

string Politiecombi::getType() const
{
    assert(properlyInit());
    return "Politiecombi";
}