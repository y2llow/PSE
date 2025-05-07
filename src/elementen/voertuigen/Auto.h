//
// Created by Abdellah on 4/28/2025.
//

#ifndef AUTO_H
#define AUTO_H
#include "../Voertuig.h"


class Auto : public Voertuig
{
    Auto* _initCheck; // Voor properlyInit

public:
    /**
     * @brief Standaard constructor voor Auto
     * @post properlyInit() == true
     */
    Auto() : Voertuig(4, 16.6, 1.44, 4.61, 4, false)
    {
        _initCheck = this;
    }

    /**
     * @brief Controleert of het object correct geïnitialiseerd is
     * @return true als dit object correct is geïnitialiseerd
     */
    bool properlyInit() const { return _initCheck == this; }

    /**
     * @brief Geeft het type van het voertuig terug
     * @pre properlyInit() == true
     * @return "Auto"
     */
    [[nodiscard]] string getType() const override;
};


#endif //AUTO_H