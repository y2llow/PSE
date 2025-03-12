//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef VERKEERSLICHT_H
#define VERKEERSLICHT_H
#include <string>


class Verkeerslicht {
    std::string baan;
    int positie;
    int cyclus;
public:
    Verkeerslicht() = default;

    Verkeerslicht(const std::string &baan, const int positie, const int cyclus)
        : baan(baan),
          positie(positie),
          cyclus(cyclus) {
    }

    [[nodiscard]] std::string getBaan() const;

    void setBaan(const std::string &baan);

    [[nodiscard]] int getPositie() const;

    void setPositie(const int positie);

    [[nodiscard]] int getCyclus() const;

    void setCyclus(const int cyclus);
};



#endif //VERKEERSLICHT_H
