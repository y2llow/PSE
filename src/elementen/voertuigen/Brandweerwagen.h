//
// Created by Abdellah on 4/28/2025.
//

#ifndef BRANDWEERWAGEN_H
#define BRANDWEERWAGEN_H
#include "../Voertuig.h"

class Brandweerwagen : public Voertuig
{
    Brandweerwagen* _initCheck; // Voor properlyInit

public:
    /**
     * @brief Standaard constructor voor Brandweerwagen
     * @post properlyInit() == true
     */
    Brandweerwagen() : Voertuig(8, 27.8, 1.8, 5.1, 3, true)
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
     * @return "Brandweerwagen"
     */
    [[nodiscard]] string getType() const override;
};


#endif //BRANDWEERWAGEN_H