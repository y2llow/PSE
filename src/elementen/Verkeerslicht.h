//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef VERKEERSLICHT_H
#define VERKEERSLICHT_H
#include <string>

#include "Baan.h"


using namespace std;

class Verkeerslicht {
    string baan;
    double positie;
    double cyclus;
    bool rood = false;
    bool groen = true;
    double tijdSindsLaatsteVerandering = 0;

public:
    string getBaan() const ;

    void setBaan(const string &baan);

    [[nodiscard]] double getPositie() const;

    void setPositie(double positie);

    [[nodiscard]] double getCyclus() const;

    void setCyclus(double cyclus);

    bool isRood() const;

    void setRood(bool rood);

    bool isGroen() const;

    void setGroen(bool groen);

    void updateVerkeersLicht();

    void updateTijdSindsLaatsteVerandering(double tijd);

    void herstartCyclus();

    double getTijdSindsLaatsteVerandering() const;
};



#endif //VERKEERSLICHT_H
