
#include "Auto.h"
#include "../../DesignByContract.h"


string Auto::getType() const
{
    REQUIRE(properlyInit(), "Bus is niet correct geïnitialiseerd");
    return "Auto";
}