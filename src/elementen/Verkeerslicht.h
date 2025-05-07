//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef VERKEERSLICHT_H
#define VERKEERSLICHT_H
#include <string>

#include "Baan.h"


class Simulator;
using namespace std;

class Verkeerslicht {
    Baan *baan{};
    double positie{};
    double cyclus{};

    //eerste double is 10%van de cyclus, 2de is de tijd na groen verandering naar rood
    pair<double,double> oranjecyclus;
    bool oranje = false;
    bool opKruispunt = false;
    bool VoertuigInVertraagzone = false;
    pair<Verkeerslicht*,Verkeerslicht*> opKruispuntPair;
    bool groen = true;

    double tijd_sinds_laatste_verandering = 0;

    Verkeerslicht* _initCheck; // Voor properlyInit

public:
    /**
     * @brief Constructor
     * @post properlyInit() == true
     */
    Verkeerslicht() {
        _initCheck = this;
    }

    /**
     * @brief Controleert of het object correct geïnitialiseerd is
     * @return true als dit object correct is geïnitialiseerd
     */
    bool properlyInit() const { return _initCheck == this; }

    /**
     * @brief Geeft de bijbehorende baan terug
     * @pre properlyInit() == true
     */
    [[nodiscard]] Baan* getBaan() const;

    /**
     * @brief Zet de baan van het verkeerslicht
     * @pre properlyInit() == true
     * @post getBaan() == baan
     */
    void setBaan(Baan* baan);

    /**
     * @brief Geeft de positie van het verkeerslicht
     * @pre properlyInit() == true
     */
    [[nodiscard]] double getPositie() const;

    /**
     * @brief Zet de positie van het verkeerslicht
     * @pre properlyInit() == true
     * @post getPositie() == positie
     */
    void setPositie(double positie);

    /**
     * @brief Geeft de cyclus van het verkeerslicht
     * @pre properlyInit() == true
     */
    [[nodiscard]] double getCyclus() const;

    /**
     * @brief Zet de cyclus van het verkeerslicht
     * @pre properlyInit() == true && cyclus > 0
     * @post getCyclus() == cyclus
     */
    void setCyclus(double cyclus);

    /**
     * @brief Controleert of het verkeerslicht op een kruispunt staat
     * @pre properlyInit() == true
     */
    bool isOpKruispunt() const;

    /**
     * @brief Zet of het verkeerslicht op een kruispunt staat
     * @pre properlyInit() == true
     * @post isOpKruispunt() == opKruispunt
     */
    void setOpKruispunt(bool opKruispunt);

    /**
     * @brief Controleert of het verkeerslicht oranje is
     * @pre properlyInit() == true
     */
    bool isOranje() const;

    /**
     * @brief Zet of het verkeerslicht oranje is
     * @pre properlyInit() == true
     * @post isOranje() == oranje
     */
    void setOranje(bool oranje);

    /**
     * @brief Controleert of het verkeerslicht groen is
     * @pre properlyInit() == true
     */
    [[nodiscard]] bool isGroen() const;

    /**
     * @brief Switcht de kleur van het verkeerslicht
     * @pre properlyInit() == true
     * @post isGroen() != oude waarde van isGroen()
     */
    void switchColor();

    /**
     * @brief Update de status van het verkeerslicht en beïnvloedt voertuigen
     * @pre properlyInit() == true && baan != nullptr
     * @post tijd_sinds_laatste_verandering is bijgewerkt
     */
    void updateVerkeerslicht();

    /**
     * @brief Switcht de kleuren van twee verkeerslichten en reset hun timers
     * @pre vk1 != nullptr && vk2 != nullptr
     * @post vk1->isGroen() != oude waarde van vk1->isGroen() && vk2->isGroen() != oude waarde van vk2->isGroen()
     */
    static void switchVerkeerslichten(Verkeerslicht* vk1, Verkeerslicht* vk2);

    /**
     * @brief Markeer verkeerslicht als op kruispunt
     * @pre properlyInit() == true
     * @post isOpKruispunt() == true
     */
    void OpKruisPunt();

    /**
     * @brief Stel kruispuntpaar in voor dit verkeerslicht
     * @pre properlyInit() == true && v1 != nullptr && v2 != nullptr
     */
    void KruispuntPair(Verkeerslicht* v1, Verkeerslicht* v2);

    /**
     * @brief Stel oranje cyclus in
     * @pre properlyInit() == true && c >= 0
     */
    void setOranjeCyclus(double c);
};


#endif //VERKEERSLICHT_H