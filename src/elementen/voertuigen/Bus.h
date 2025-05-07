//
// Created by Abdellah on 4/28/2025.
//

#ifndef BUS_H
#define BUS_H
#include "../Voertuig.h"

class Bus : public Voertuig
{
    double tijd_sinds_stopped = 0;
    Bus* _initCheck; // Voor properlyInit

public:
    /**
     * @brief Standaard constructor voor Bus
     * @post properlyInit() == true
     */
    Bus() : Voertuig(12, 13.9, 1.0, 2.9, 6, false)
    {
        _initCheck = this;
    }

    /**
     * @brief Controleert of het object correct geïnitialiseerd is
     * @return true als dit object correct is geïnitialiseerd
     */
    bool properlyInit() const { return _initCheck == this; }

    /**
     * @brief Geeft de tijd sinds het voertuig is gestopt
     * @pre properlyInit() == true
     * @return Tijd sinds laatst gestopt
     */
    [[nodiscard]] double getTimeSindsStopped() const;

    /**
     * @brief Zet de tijd sinds het voertuig is gestopt
     * @pre properlyInit() == true
     * @post getTimeSindsStopped() == t
     */
    void setTimeSindsStopped(double t);

    /**
     * @brief Geeft het type van het voertuig terug
     * @pre properlyInit() == true
     * @return "Bus"
     */
    [[nodiscard]] string getType() const override;
};


#endif //BUS_H