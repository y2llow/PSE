//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef BAAN_H
#define BAAN_H
#include <string>
#include <map>

class Baan {
    std::string naam;
    int lengte;

public:
    //eerste int in de pair is voor this->position en 2de int is voor baan->position
    std::map< Baan* , std::pair<int,int> > kruispunten;
    Baan() = default;

    Baan(const std::string &naam, const int lengte)
        : naam(naam),
          lengte(lengte) {
    }

    [[nodiscard]] std::string getNaam() const;

    [[nodiscard]] int getLengte() const;

    void setNaam(const std::string &naam);

    void setLengte(const int lengte);


};



#endif //BAAN_H
