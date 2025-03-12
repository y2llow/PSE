//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef VOERTUIGGENERATOR_H
#define VOERTUIGGENERATOR_H
#include <string>


class Voertuiggenerator {
    std::string baan;
    int frequentie = 0;

public:
    Voertuiggenerator() = default;

    Voertuiggenerator(const std::string &baan, const int frequentie)
        : baan(baan),
          frequentie(frequentie) {
    }

    [[nodiscard]] std::string getBaan() const;

    void setBaan(const std::string &baan);

    [[nodiscard]] int getFrequentie() const;

    void setFrequentie(int frequentie);
};


#endif //VOERTUIGGENERATOR_H
