//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef VOERTUIG_H
#define VOERTUIG_H
#include <string>
#include <vector>

#include "Baan.h"


class Voertuig {
    Baan baan;
    double positie;
    double snelheid;
    int id;
    double simulationTimeinc = 0.0166;


    double Vmax = 16.6;
    double amax = 1.44;
    double bmax = 4.61;
    double fmin = 4;
    double vertraagAfstand = 50;
    double stopAfstand = 15;
    double vertraagFactor = 0.4;
    double length = 4;
    double versnelling = 0;

public:
    Voertuig() = default;

    Voertuig(const Baan &baan, const int positie)
        : baan(baan),
          positie(positie) {
    }

    Baan getBaan() const ;

    void setBaan(const Baan &weg);

    double getPositie() const;

    void setPositie(double positie);

    double getSnelheid() const;

    void setSnelheid(double snelheid);

    int getId() const;

    void setId(int id);

    double getVersnelling() const;

    void setVersnelling(double versnelling);

    double getLength() const;

    void BerekenPositieVoertuig() ;

    void BerekenVersnellingVoertuig(std::size_t counter , std::vector<Voertuig*> voertuigen) ;
};



#endif //VOERTUIG_H
