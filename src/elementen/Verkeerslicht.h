//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef VERKEERSLICHT_H
#define VERKEERSLICHT_H
#include <string>

#include "Baan.h"


class Verkeerslicht {
public:
    Baan baan;
    double positie;
    double cyclus;
    bool rood = false;
    bool groen = true;
    double tijdSindsLaatsteVerandering = 0;


    Baan getBaan() const ;

    void setBaan(const Baan &baan);

    [[nodiscard]] double getPositie() const;

    void setPositie(double positie);

    [[nodiscard]] double getCyclus() const;

    void setCyclus(double cyclus);

    bool isRood() const;

    void setRood(bool rood);

    bool isGroen() const;

    void setGroen(bool groen);

    void UpdateVerkeersLicht();

    void UpdateTijdSindsLaatsteVerandering(double tijd);

    void herstartCyclus();

    double getTijdSindsLaatsteVerandering() const;
};



#endif //VERKEERSLICHT_H
