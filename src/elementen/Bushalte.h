#ifndef BUSHALTE_H
#define BUSHALTE_H

#include <vector>
#include <cassert>

class Baan;
class Bus;

class Bushalte {
    std::vector<Bus*> waited_busses;
    Baan* baan{};
    double positie{};
    double wachttijd{};
    Bushalte* initCheck;

public:
    /**
     * @brief Constructor
     * Postconditie: properlyInit() == true
     */
    Bushalte() {
        initCheck = this;
        assert(properlyInit());
    }

    bool properlyInit() const {
        return initCheck == this;
    }

    /**
     * @brief Geeft de bijbehorende baan terug.
     * Preconditie: properlyInit() == true
     */
    [[nodiscard]] Baan* getBaan() const;

    /**
     * @brief Zet de baan.
     * Preconditie: properlyInit() == true && baan != nullptr
     * Postconditie: getBaan() == baan
     */
    void setBaan(Baan* baan);

    /**
     * @brief Geeft de positie van de bushalte.
     * Preconditie: properlyInit() == true
     */
    [[nodiscard]] double getPositie() const;

    /**
     * @brief Zet de positie van de bushalte.
     * Preconditie: properlyInit() == true && positie >= 0
     * Postconditie: getPositie() == positie
     */
    void setPositie(double positie);

    /**
     * @brief Geeft de wachttijd van de bushalte.
     * Preconditie: properlyInit() == true
     */
    [[nodiscard]] double getWachttijd() const;

    /**
     * @brief Zet de wachttijd van de bushalte.
     * Preconditie: properlyInit() == true && cyclus >= 0
     * Postconditie: getWachttijd() == cyclus
     */
    void setWachttijd(double cyclus);

    /**
     * @brief Laat een bus stoppen of versnellen aan deze halte.
     * Preconditie: properlyInit() == true && baan != nullptr
     * Postconditie: waited_busses bevat maximaal 1 extra bus
     */
    void stopBus();
};

#endif // BUSHALTE_H
