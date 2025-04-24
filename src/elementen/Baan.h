//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef BAAN_H
#define BAAN_H
#include <string>
#include <vector>
#include <map>
class Verkeerslicht;
class Bushalte;

class Baan {
    std::string naam;

    //de baan moet de verkeerslichten bevatten (dit staat vast aan een baan)
    std::vector<Verkeerslicht*> verkeerslichten;

    //de baan moet de bushaltes bevatten (dit staat vast aan een baan)
    std::vector<Bushalte*> bushaltes;

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

    void VulVerkeerslichtenVector(Verkeerslicht* VL);

    void VulBushaltesVector(Bushalte* BH);

    const std::vector<Verkeerslicht *> &getVerkeerslichten() const;

    const std::vector<Bushalte *> &getBushaltes() const;


};



#endif //BAAN_H
