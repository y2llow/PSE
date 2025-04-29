//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef VOERTUIGGENERATOR_H
#define VOERTUIGGENERATOR_H
#include <string>
#include <utility>

#include "Baan.h"


class Voertuiggenerator
{
    Baan* baan{};
    double frequentie{};
    string type;

    double tijd_sinds_laatste_voertuig = 0;

public:
    Voertuiggenerator() = default;

    Voertuiggenerator(Baan* baan, const double frequentie, string  type)
        : baan(baan),
          frequentie(frequentie),
          type(std::move(type))
    {
    }


    [[nodiscard]] Baan* getBaan() const;
    void setBaan(Baan* b);

    [[nodiscard]] string getType() const;

    [[nodiscard]] double getFrequentie() const;
    void setFrequentie(double frequentie);

    [[nodiscard]] string getType();

    void generateVoertuig();
};


#endif //VOERTUIGGENERATOR_H
