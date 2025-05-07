//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef VOERTUIGGENERATOR_H
#define VOERTUIGGENERATOR_H
#include <string>
#include <utility>

#include "Baan.h"


class Voertuiggenerator
{
    Baan* baan{};
    double frequentie{};
    string type;

    double tijd_sinds_laatste_voertuig = 0;

    Voertuiggenerator* _initCheck; // Voor properlyInit

public:
    /**
     * @brief Standaard constructor
     * @post properlyInit() == true
     */
    Voertuiggenerator() {
        _initCheck = this;
    }

    /**
     * @brief Constructor met parameters
     * @post properlyInit() == true && getBaan() == baan && getFrequentie() == frequentie
     */
    Voertuiggenerator(Baan* baan, const double frequentie, string  type)
            : baan(baan),
              frequentie(frequentie),
              type(std::move(type))
    {
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
     * @brief Zet de baan van de generator
     * @pre properlyInit() == true
     * @post getBaan() == b
     */
    void setBaan(Baan* b);

    /**
     * @brief Geeft het type terug
     * @pre properlyInit() == true
     */
    [[nodiscard]] string getType() const;

    /**
     * @brief Geeft de frequentie terug
     * @pre properlyInit() == true
     */
    [[nodiscard]] double getFrequentie() const;

    /**
     * @brief Zet de frequentie van de generator
     * @pre properlyInit() == true && frequentie >= 0
     * @post getFrequentie() == frequentie
     */
    void setFrequentie(double frequentie);

    /**
     * @brief Geeft het type terug (overload)
     * @pre properlyInit() == true
     */
    [[nodiscard]] string getType();

    /**
     * @brief Genereert een voertuig volgens het gedefinieerde type
     * @pre properlyInit() == true && baan != nullptr
     * @post Als nieuw voertuig gegenereerd: tijd_sinds_laatste_voertuig == 0
     * @post Als geen nieuw voertuig gegenereerd: tijd_sinds_laatste_voertuig += SIMULATIE_TIJD
     */
    void generateVoertuig();
};


#endif //VOERTUIGGENERATOR_H