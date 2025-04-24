//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef BAAN_H
#define BAAN_H
#include <string>
#include <map>

class Baan {
    std::string naam;
    std::map< std::string , std::pair<int,int> > kruispunten;
    int lengte;

public:
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
