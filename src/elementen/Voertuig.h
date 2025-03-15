//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef VOERTUIG_H
#define VOERTUIG_H
#include <string>
#include <vector>

#include "Baan.h"


using namespace std;

class Voertuig {
    string baan;
    double positie;
    double snelheid;
    int id;
    double simulationTimeinc = 0.0166;
    double length = 4;
    double versnelling = 0;
    double gVmax = 16.6;
    double kvmax = 0;
    double amax = 1.44;
    double bmax = 4.61;
    double a = 0;
    double vertraagFactor = 0.4;

public:
    Voertuig() = default;

    Voertuig(const string &baan, const int positie)
        : baan(baan),
          positie(positie) {
    }

    string getBaan() const ;

    void setBaan(const string &weg);

    double getPositie() const;

    void setPositie(double positie);

    double getSnelheid() const;

    void setSnelheid(double snelheid);

    int getId() const;

    void setId(int id);

    double getVersnelling() const;

    void setVersnelling(double versnelling);

    double getLength() const;

    double getGVmax() const;

    void setGVmax(double gVmax);

    double getKvmax() const;

    void setKvmax(double kvmax);

    double getAmax() const;

    void setAmax(double amax);

    double getBmax() const;

    void setBmax(double bmax);

    double getVertraagFactor() const;

    void setVertraagFactor(double vertraagFactor);

    double getA() const;

    void setA(double a);

    void UpdateVersnellingVoorStoppen();





/*
    void BerekenPositieVoertuig() ;

    void BerekenVersnellingVoertuig(std::size_t counter , std::vector<Voertuig*> voertuigen) ;
*/};



#endif //VOERTUIG_H
