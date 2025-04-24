#ifndef BUSHALTE_H
#define BUSHALTE_H
#include <string>
#include "Baan.h"
using namespace std;

class Bushalte {
    Baan *baan;
    double positie;
    double wachttijd;

public:
    Bushalte() = default;

    [[nodiscard]] Baan* getBaan() const;

    void setBaan(Baan* baan);

    [[nodiscard]] double getPositie() const;

    void setPositie(double positie);

    double getWachttijd() const;

    void setWachttijd(double cyclus);
};



#endif //BUSHALTE_H
