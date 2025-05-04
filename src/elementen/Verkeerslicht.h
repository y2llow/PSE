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

    //eerste double is 10%van de cyclus, 2de is de tijd na groen verandering naar rood
    pair<double,double> oranjecyclus;
    bool oranje = false;
    bool opKruispunt = false;
    pair<Verkeerslicht*,Verkeerslicht*> opKruispuntPair;
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

    void setOranjeCyclus(double c);
};


#endif //VERKEERSLICHT_H
