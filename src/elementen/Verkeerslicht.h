//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef VERKEERSLICHT_H
#define VERKEERSLICHT_H
#include <string>


class Verkeerslicht {
public:
    std::string baan;
    double positie;
    double cyclus;


    [[nodiscard]] std::string getBaan() const;

    void setBaan(const std::string &baan);

    [[nodiscard]] double getPositie() const;

    void setPositie(double positie);

    [[nodiscard]] double getCyclus() const;

    void setCyclus(double cyclus);
};



#endif //VERKEERSLICHT_H
