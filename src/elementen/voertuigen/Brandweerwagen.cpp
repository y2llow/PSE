//
// Created by Abdellah on 4/28/2025.
//

#include "Brandweerwagen.h"
#include "../../DesignByContract.h"

string Brandweerwagen::getType() const
{
    REQUIRE(properlyInit(), "Bus is niet correct geïnitialiseerd");
    return "Brandweerwagen";
}