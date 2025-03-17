//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef VOERTUIGGENERATOR_H
#define VOERTUIGGENERATOR_H
#include <string>

#include "Baan.h"


class Voertuiggenerator {
    Baan* baan;
    double frequentie;

public:
    Baan* getBaan() const;

    void setBaan(Baan* b);

    double getFrequentie() const;

    void setFrequentie(const double frequentie);
};



#endif //VOERTUIGGENERATOR_H
