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
    /**
     * @brief Constructor voor Kruispunt
     * @pre Elke baan in de map moet ook aanwezig zijn in de vector bannen
     * @post Object is correct geïnitialiseerd
     */
    Kruispunt(const vector<Baan*>& b, const map<Baan*, int>& p);

    /**
     * @brief Geeft de bannen op het kruispunt
     * @return vector van Baan pointers
     * @post Originele bannen blijven onveranderd
     */
    [[nodiscard]] const vector<Baan*> getBannen() const;

    /**
     * @brief Geeft de posities op het kruispunt
     * @return map van Baan* naar int
     * @post Originele map blijft onveranderd
     */
    [[nodiscard]] const map<Baan*, int> getPositions() const;

    /**
     * @brief Voeg een baan toe aan het kruispunt
     * @pre baan != nullptr
     * @post Baan wordt toegevoegd aan de bannen-lijst
     */
    void addBaan(Baan* baan);

    /**
     * @brief Voeg een positie toe voor een baan
     * @pre baan != nullptr
     * @post Positie wordt toegevoegd of geüpdatet in de map
     */
    void addPosition(Baan* baan, int position);

    /**
     * @brief Controleert of alle banen op het kruispunt een verkeerslicht hebben op de juiste positie
     * @return Tuple: true als alle lichten aanwezig zijn, plus vector met de gevonden verkeerslichten
     * @post Objectstatus verandert niet
     */
    [[nodiscard]] tuple<bool, vector<Verkeerslicht*>> verkeerslichtenStaanOpKruispunt() const;

    /**
     * @brief Geeft tijd sinds laatste verandering
     * @return double waarde
     */
    [[nodiscard]] double getTimeSince() const;

    /**
     * @brief Verhoogt de tijd sinds laatste verandering
     * @pre i >= 0
     * @post time_since_last_change wordt aangepast
     */
    void increaseTimeSince(double i);

    /**
     * @brief Zet de tijd sinds laatste verandering
     * @pre t >= 0
     * @post time_since_last_change == t
     */
    void setTimeSince(double t);

    /**
     * @brief Maakt een nieuw Kruispunt-object aan
     * @pre bannen en positions mogen geen nullpointers bevatten
     * @post Retourneert een nieuw gealloceerd Kruispunt-object
     */
    static Kruispunt* createNewKruispunt(const vector<Baan*>& bannen, const map<Baan*, int>& positions);

    /**
     * @brief Zet het actieve verkeerslicht
     * @pre verkeerslicht != nullptr
     * @post Alleen verkeerslicht staat op groen, andere op oranje/rood
     */
    void setActiveVerkeerslicht(Verkeerslicht* verkeerslicht);

    /**
     * @brief Haalt het actieve verkeerslicht op
     * @return Pointer naar actief verkeerslicht, of nullptr
     */
    Verkeerslicht* getActiveVerkeerslicht();
};

#endif // KRUISPUNT_H



