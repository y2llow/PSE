//
// Created by Abdellah on 4/28/2025.
//

#ifndef ZIEKENWAGEN_H
#define ZIEKENWAGEN_H
#include "../Voertuig.h"

class Ziekenwagen : public Voertuig
{
    Ziekenwagen* _initCheck; // Voor properlyInit

public:
    /**
     * @brief Standaard constructor voor Ziekenwagen
     * @post properlyInit() == true
     */
    Ziekenwagen() : Voertuig(6, 24.1, 1.7, 5.0, 2, true)
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
     * @return "Ziekenwagen"
     */
    [[nodiscard]] string getType() const override;
};


#endif //ZIEKENWAGEN_H