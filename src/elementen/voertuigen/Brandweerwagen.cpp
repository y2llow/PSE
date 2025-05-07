//
// Created by Abdellah on 4/28/2025.
//

#include "Brandweerwagen.h"
#include <cassert>

string Brandweerwagen::getType() const
{
    assert(properlyInit());
    return "Brandweerwagen";
}