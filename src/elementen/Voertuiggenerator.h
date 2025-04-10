//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef VOERTUIGGENERATOR_H
#define VOERTUIGGENERATOR_H
#include <string>

#include "Baan.h"
#include "TypeVoertuig.h"


class Voertuiggenerator {
    Baan* baan;
    double frequentie;
    // Nieuw veld voor het type voertuig
    TypeVoertuig typeVoertuig;
    VoertuigType type;

public:
    Voertuiggenerator() = default;

    Voertuiggenerator(Baan* baan, double frequentie, VoertuigType type = VoertuigType::AUTO)
        : baan(baan),
          frequentie(frequentie),
          type(type){
        typeVoertuig = TypeVoertuig::createVoertuigType(type);
    }


    Baan* getBaan() const;

    void setBaan(Baan* b);

    double getFrequentie() const;

    void setFrequentie(const double frequentie);

    VoertuigType getType() const {
        return type;
    }
};



#endif //VOERTUIGGENERATOR_H
