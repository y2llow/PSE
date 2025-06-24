#ifndef VOERTUIG_H
#define VOERTUIG_H
#include <string>

#include "Baan.h"

class ErrorOutput;
class Simulator;
using namespace std;

inline int GENERAL_ID = 1;

enum class State
{
    DRIVING,
    SLOWINGDOWN,
    STOPPING
};

class Voertuig
{
    int id = GENERAL_ID;
    Baan* baan = nullptr;
    double l{};
    double p = 0;
    double v = 0;
    double a = 0;
    double v_max{};
    double a_max{};
    double b_max{};
    double f_min{};
    double k_v_max = 0;

    bool prioriteitsvoertuig = false;

    State voertuig_state = State::DRIVING;

    Voertuig* _initCheck; // Voor properlyInit

public:
    /**
     * @brief Controleert of het object correct geïnitialiseerd is
     * @return true als dit object correct is geïnitialiseerd
     */
    bool properlyInit() const { return _initCheck == this; }

    virtual ~Voertuig() = default;

    /**
     * @brief Constructor voor een voertuig
     * @post properlyInit() == true
     */
    Voertuig(const double lengte, const double maximale_snelheid, const double maximale_versnelling, const double maximale_remfactor,
             const double minimale_volgaftand, const bool prioriteit):
            l(lengte), v_max(maximale_snelheid), a_max(maximale_versnelling), b_max(maximale_remfactor),
            f_min(minimale_volgaftand), k_v_max(maximale_snelheid), prioriteitsvoertuig(prioriteit)
    {
        id = GENERAL_ID;
        GENERAL_ID++;
        _initCheck = this;
    }

    /**
     * @brief Geeft de bijbehorende baan terug
     * @pre properlyInit() == true
     */
    [[nodiscard]] Baan* getBaan() const;

    /**
     * @brief Zet de baan van het voertuig
     * @pre properlyInit() == true
     * @post getBaan() == weg
     */
    void setBaan(Baan* weg);

    /**
     * @brief Geeft de positie van het voertuig terug
     * @pre properlyInit() == true
     */
    [[nodiscard]] double getPositie() const;

    /**
     * @brief Zet de positie van het voertuig
     * @pre properlyInit() == true
     * @post getPositie() == positie
     */
    void setPositie(double positie);

    /**
     * @brief Geeft de snelheid van het voertuig terug
     * @pre properlyInit() == true
     */
    [[nodiscard]] double getSnelheid() const;

    /**
     * @brief Zet de snelheid van het voertuig
     * @pre properlyInit() == true
     * @post getSnelheid() == speed
     */
    void setSnelheid(double speed);

    /**
     * @brief Geeft de huidige toestand van het voertuig terug
     * @pre properlyInit() == true
     */
    [[nodiscard]] State getState() const;

    /**
     * @brief Geeft het ID van het voertuig terug
     * @pre properlyInit() == true
     */
    [[nodiscard]] int getId() const;

    /**
     * @brief Zet het ID van het voertuig
     * @pre properlyInit() == true
     * @post getId() == id
     */
    void setId(int id);

    /**
     * @brief Geeft de versnelling van het voertuig terug
     * @pre properlyInit() == true
     */
    [[nodiscard]] double getVersnelling() const;

    /**
     * @brief Zet de versnelling van het voertuig
     * @pre properlyInit() == true
     * @post getVersnelling() == afstand
     */
    void setVersnelling(double afstand);

    /**
     * @brief Geeft de huidige maximale snelheid met factor terug
     * @pre properlyInit() == true
     */
    [[nodiscard]] double getKvmax() const;

    /**
     * @brief Zet de huidige maximale snelheid met factor
     * @pre properlyInit() == true
     * @post getKvmax() == kvmax
     */
    void setKvmax(double kvmax);

    /**
     * @brief Geeft de lengte van het voertuig terug
     * @pre properlyInit() == true
     */
    [[nodiscard]] double getLengte() const;

    /**
     * @brief Zet de lengte van het voertuig
     * @pre properlyInit() == true
     * @post getLengte() == lengte
     */
    void setLengte(double lengte);

    /**
     * @brief Geeft de maximale snelheid van het voertuig terug
     * @pre properlyInit() == true
     */
    [[nodiscard]] double getMaximaleSnelheid() const;

    /**
     * @brief Zet de maximale snelheid van het voertuig
     * @pre properlyInit() == true
     * @post getMaximaleSnelheid() == maximale_snelheid
     */
    void setMaximaleSnelheid(double maximale_snelheid);

    /**
     * @brief Geeft de maximale versnelling van het voertuig terug
     * @pre properlyInit() == true
     */
    [[nodiscard]] double getMaximaleVersnelling() const;

    /**
     * @brief Zet de maximale versnelling van het voertuig
     * @pre properlyInit() == true
     * @post getMaximaleVersnelling() == maximale_versnelling
     */
    void setMaximaleVersnelling(double maximale_versnelling);

    /**
     * @brief Geeft de maximale remfactor van het voertuig terug
     * @pre properlyInit() == true
     */
    [[nodiscard]] double getMaximaleRemfactor() const;

    /**
     * @brief Zet de maximale remfactor van het voertuig
     * @pre properlyInit() == true
     * @post getMaximaleRemfactor() == maximale_remfactor
     */
    void setMaximaleRemfactor(double maximale_remfactor);

    /**
     * @brief Geeft de minimale volgafstand van het voertuig terug
     * @pre properlyInit() == true
     */
    [[nodiscard]] double getMinimaleVolgafstand() const;

    /**
     * @brief Zet de minimale volgafstand van het voertuig
     * @pre properlyInit() == true
     * @post getMinimaleVolgafstand() == minimale_volgaftand
     */
    void setMinimaleVolgafstand(double minimale_volgaftand);

    /**
     * @brief Controleert of het voertuig vertraagd is
     * @pre properlyInit() == true
     */
    [[nodiscard]] bool isSlowed() const;

    /**
     * @brief Zet de vertraagstatus van het voertuig
     * @pre properlyInit() == true
     */
    void setSlowed(bool is_slowed);

    /**
     * @brief Controleert of het voertuig een prioriteitsvoertuig is
     * @pre properlyInit() == true
     */
    [[nodiscard]] bool isPrioriteitsVoertuig() const;

    /**
     * @brief Geeft het type van het voertuig terug
     * @pre properlyInit() == true
     */
    [[nodiscard]] virtual string getType() const;

    /**
     * @brief Zet de toestand van het voertuig
     * @pre properlyInit() == true
     * @post getState() == state
     */
    void setState(State state);

    /**
     * @brief Update de versnelling voor het stoppen
     * @pre properlyInit() == true
     */
    void updateVersnellingVoorStoppen();

    /**
     * @brief Creëert een voertuig van het gegeven type
     * @return een pointer naar een nieuw voertuig
     */
    static Voertuig* createVoertuig(const string& type, ErrorOutput* errorOutput);
    static Voertuig* createVoertuig(const string& type);

    /**
     * @brief Laat het voertuig rijden
     * @pre properlyInit() == true && baan != nullptr
     * @post Positie en snelheid zijn bijgewerkt volgens rijgedrag
     */
    virtual void rijd();

    /**
     * @brief Laat het voertuig vertragen
     * @pre properlyInit() == true
     * @post getState() == State::SLOWINGDOWN && getKvmax() == VERTRAAG_FACTOR * getMaximaleSnelheid()
     */
    void slowDown();

    /**
     * @brief Laat het voertuig versnellen
     * @pre properlyInit() == true
     * @post getState() == State::DRIVING && getKvmax() == getMaximaleSnelheid()
     */
    void accelerate();

    /**
     * @brief Laat het voertuig stoppen
     * @pre properlyInit() == true
     * @post getState() == State::STOPPING
     */
    void stop();

    /**
     * @brief Controleert of het voertuig een kruispunt passeert
     * @pre properlyInit() == true
     */
    [[nodiscard]] bool opKruispunt(double old_position) const;

    /**
     * @brief Verwerkt een kruispunt voor het voertuig
     * @pre properlyInit() == true
     */
    void chooseKruispunt();
};

#endif //VOERTUIG_H

