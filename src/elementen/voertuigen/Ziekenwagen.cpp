//
// Created by Abdellah on 4/28/2025.
//

#include "Ziekenwagen.h"
#include "../../DesignByContract.h"

string Ziekenwagen::getType() const
{
    REQUIRE(properlyInit(), "Bus is niet correct geïnitialiseerd");
    return "Ziekenwagen";
}