//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef VERKEERSLICHT_H
#define VERKEERSLICHT_H
#include <string>

#include "Baan.h"


class Kruispunt;
class Simulator;
using namespace std;

enum class LightState
{
    GREEN, ORANGE, RED
};

class Verkeerslicht {
    Baan *baan{};
    double positie{};
    double cyclus{};

    LightState state = LightState::GREEN;
    double tijd_sinds_laatste_verandering = 0;

    Kruispunt* kruispunt = nullptr;

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

    [[nodiscard]] Kruispunt* getKruispunt() const;
    void setKruispunt(Kruispunt* kruispunt);

    [[nodiscard]] LightState getState() const;
    void setState(LightState state);


    int getWaitingVehicles() const;

    void setTimeSince(int i);
    double getTimeSince();

};


#endif //VERKEERSLICHT_H