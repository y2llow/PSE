
#include "Auto.h"
#include <cassert>


string Auto::getType() const
{
    assert(properlyInit());
    return "Auto";
}