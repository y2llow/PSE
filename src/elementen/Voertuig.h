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
    double versnelling = 0;
    double kvmax = 0;

public:
    Voertuig() = default;

    Voertuig(const string &baan, const int positie)
        : baan(baan),
          positie(positie) {
    }

    string getBaan() const;

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

    double getKvmax() const;

    void setKvmax(double kvmax);

    void UpdateVersnellingVoorStoppen();
};


#endif //VOERTUIG_H
