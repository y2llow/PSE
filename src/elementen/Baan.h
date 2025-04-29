//
// Created by Abdellah on 4/28/2025.
//

#ifndef BAAN_H
#define BAAN_H
#include <string>
#include <utility>
#include <vector>
#include <map>

class Voertuiggenerator;
class Voertuig;
using namespace std;

class Verkeerslicht;
class Bushalte;

class Baan
{
    string naam;
    int lengte{};

    vector<Bushalte*> bushaltes;
    vector<Verkeerslicht*> verkeers;
    vector<Voertuig*> voertuigen;
    vector<Voertuiggenerator*> voertuiggeneratoren;

    vector<double> bushaltes_positions;

public:
    map<int, vector<Baan*>> kruispunten;

    //eerste int in de pair is voor this->position en 2de int is voor baan->position
    Baan() = default;

    Baan(string naam, const int lengte)
        : naam(move(naam)),
          lengte(lengte)
    {
    }

    [[nodiscard]] string getNaam() const;
    void setNaam(const string& naam);

    [[nodiscard]] int getLengte() const;
    void setLengte(int lengte);


    void addBushalte(Bushalte* b);
    void addVerkeerslicht(Verkeerslicht* v);
    void addVoertuig(Voertuig* v);
    void addVoertuiggenerator(Voertuiggenerator* vg);

    void removeVoertuig(Voertuig* v);

    const vector<Verkeerslicht *> &getVerkeerslichten() const;
    const vector<Bushalte *> &getBushaltes() const;
    const vector<Voertuig *> &getVoertuigen() const;
    const vector<Voertuiggenerator *> &getVoertuigeneratoren() const;

    void sortVoertuigenByPosition();


};


#endif //BAAN_H
