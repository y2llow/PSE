//
// Created by Abdellah on 4/28/2025.
//

#include "Ziekenwagen.h"
#include <cassert>

string Ziekenwagen::getType() const
{
    assert(properlyInit());
    return "Ziekenwagen";
}