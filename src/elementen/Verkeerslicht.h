//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef VERKEERSLICHT_H
#define VERKEERSLICHT_H
#include <string>

#include "Baan.h"


class Simulator;
using namespace std;

class Verkeerslicht {
    Baan *baan{};
    double positie{};
    double cyclus{};
    bool verkeerslichtOpKruispunt = false;
    pair<Verkeerslicht*,Verkeerslicht*> verkeerlichtenOpKruispuntPair;
    bool groen = true;

    double tijd_sinds_laatste_verandering = 0;

public:
    [[nodiscard]] Baan* getBaan() const;
    void setBaan(Baan* baan);

    [[nodiscard]] double getPositie() const;
    void setPositie(double positie);

    [[nodiscard]] double getCyclus() const;
    void setCyclus(double cyclus);

    [[nodiscard]] bool isGroen() const;

    void switchColor();

    void updateVerkeerslicht();

    void OpKruisPunt();

    void KruispuntPair(Verkeerslicht* v1, Verkeerslicht* v2);
};


#endif //VERKEERSLICHT_H
