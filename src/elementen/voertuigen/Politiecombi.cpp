//
// Created by Abdellah on 4/28/2025.
//

#include "Politiecombi.h"
#include "../../DesignByContract.h"

string Politiecombi::getType() const
{
    REQUIRE(properlyInit(), "Bus is niet correct geïnitialiseerd");
    return "Politiecombi";
}