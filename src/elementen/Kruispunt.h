//
// Created by Abdellah on 5/9/2025.
//

#ifndef KRUISPUNT_H
#define KRUISPUNT_H
#include <map>
#include <utility>
#include <vector>
#include <bits/basic_ios.h>

class Verkeerslicht;
class Baan;
using namespace std;

class Kruispunt
{
    vector<Baan*> bannen;
    map<Baan*, int> positions;

    Verkeerslicht* active_verkeerslicht_ = nullptr;

    double time_since_last_change = 0;

public:
    // Kruispunt() = default;

    Kruispunt(const vector<Baan*>& b, const map<Baan*, int>& p): bannen(b), positions(p)
    {
    };

    [[nodiscard]] const vector<Baan*> getBannen() const;
    [[nodiscard]] const map<Baan*, int> getPositions() const;

    void addBaan(Baan* baan);
    void addPosition(Baan* baan, int position);
    [[nodiscard]] tuple<bool, vector<Verkeerslicht*>> verkeerslichtenStaanOpKruispunt() const;

    [[nodiscard]] double getTimeSince() const;
    void increaseTimeSince(double i);
    void setTimeSince(double t);

    /**
     * @brief creates a new instance of Kruispunt
     * @pre you should provide it with the bannen and positions
     ***/
    static Kruispunt* createNewKruispunt(const vector<Baan*>& bannen, const map<Baan*, int>& positions);


    void setActiveVerkeerslicht(Verkeerslicht* verkeerslicht);
    Verkeerslicht* getActiveVerkeerslicht();
};


#endif //KRUISPUNT_H
