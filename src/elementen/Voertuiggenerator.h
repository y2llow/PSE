//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef VOERTUIGGENERATOR_H
#define VOERTUIGGENERATOR_H
#include <string>


class Voertuiggenerator {
    std::string baan;
    double frequentie;

public:
    std::string getBaan() const;

    void setBaan(const std::string &baan);

    double getFrequentie() const;

    void setFrequentie(const double frequentie);
};



#endif //VOERTUIGGENERATOR_H
