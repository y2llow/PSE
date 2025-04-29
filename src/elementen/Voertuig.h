#ifndef VOERTUIG_H
#define VOERTUIG_H
#include <string>

#include "Baan.h"

class Simulator;
using namespace std;

inline int GENERAL_ID = 1;

enum class State
{
    DRIVING,
    STOPPING
};

class Voertuig
{
    int id = GENERAL_ID;
    Baan* baan = nullptr;
    double l{};
    double p{};
    double v = 16.6;
    double a = 0;
    double v_max{};
    double a_max{};
    double b_max{};
    double f_min{};
    double k_v_max{};

    bool is_slowed = false;
    bool prioriteitsvoertuig = false;

    State voertuig_state = State::DRIVING;

public:
    virtual ~Voertuig() = default;

    Voertuig(const double lengte, const double maximale_snelheid, const double maximale_versnelling, const double maximale_remfactor,
             const double minimale_volgaftand, const bool prioriteit):
        l(lengte), v_max(maximale_snelheid), a_max(maximale_versnelling), b_max(maximale_remfactor),
        f_min(minimale_volgaftand), k_v_max(maximale_snelheid), prioriteitsvoertuig(prioriteit)
    {
        id = GENERAL_ID;
        GENERAL_ID++;
    }

    [[nodiscard]] Baan* getBaan() const;
    void setBaan(Baan* weg);

    [[nodiscard]] double getPositie() const;
    void setPositie(double positie);

    [[nodiscard]] double getSnelheid() const;
    void setSnelheid(double speed);

    [[nodiscard]] int getId() const;
    void setId(int id);

    [[nodiscard]] double getVersnelling() const;
    void setVersnelling(double afstand);

    [[nodiscard]] double getKvmax() const;
    void setKvmax(double kvmax);

    [[nodiscard]] double getLengte() const;
    void setLengte(double lengte);

    [[nodiscard]] double getMaximaleSnelheid() const;
    void setMaximaleSnelheid(double maximale_snelheid);

    [[nodiscard]] double getMaximaleVersnelling() const;
    void setMaximaleVersnelling(double maximale_versnelling);

    [[nodiscard]] double getMaximaleRemfactor() const;
    void setMaximaleRemfactor(double maximale_remfactor);

    [[nodiscard]] double getMinimaleVolgafstand() const;
    void setMinimaleVolgafstand(double minimale_volgaftand);

    [[nodiscard]] bool isSlowed() const;
    void setSlowed(bool is_slowed);

    [[nodiscard]] bool isPrioriteitsVoertuig() const;

    [[nodiscard]] virtual string getType() const;

    void setState(State state);

    void updateVersnellingVoorStoppen();

    static Voertuig* createVoertuig(const string& type);

    virtual void rijd();
    void slowDown();
    void accelerate();
    void stop();
};

#endif //VOERTUIG_H

/*
#ifndef VOERTUIG_H
#define VOERTUIG_H

#include <string>
#include "Baan.h"

class Voertuig {
protected:
    Baan* baan;
    double positie;
    double snelheid;
    int id;
    double versnelling;
    double kvmax;

    // Default values for base class (should be overridden by derived classes)
    virtual double getDefaultLength() const { return 0.0; }
    virtual double getDefaultMaxSnelheid() const { return 0.0; }
    virtual double getDefaultMaxVersnelling() const { return 0.0; }
    virtual double getDefaultMaxRemfactor() const { return 0.0; }
    virtual double getDefaultMinVolgafstand() const { return 0.0; }

public:
    Voertuig(Baan* baan = nullptr, double positie = 0.0, int id = 0);
    virtual ~Voertuig() = default;

    // Getters and setters
    Baan* getBaan() const;
    void setBaan(Baan* weg);
    double getPositie() const;
    void setPositie(double positie);
    double getSnelheid() const;
    void setSnelheid(double snelheid);
    int getId() const;
    void setId(int id);
    double getVersnelling() const;
    void setVersnelling(double versnelling);
    double getKvmax() const;
    void setKvmax(double kvmax);

    // Vehicle-specific getters
    virtual double getLength() const;
    virtual double getMaxSnelheid() const;
    virtual double getMaxVersnelling() const;
    virtual double getMaxRemfactor() const;
    virtual double getMinVolgafstand() const;

    // Simulation methods
    virtual void updateVersnellingVoorStoppen();
    virtual void simulateStep();
};

class Auto : public Voertuig {
protected:
    static constexpr double LENGTE = 4.0;
    static constexpr double MAX_SNELHEID = 16.6;
    static constexpr double MAX_VERSNELLING = 1.44;
    static constexpr double MAX_REMFACTOR = 4.61;
    static constexpr double MIN_VOLGAFSTAND = 4.0;

    double getDefaultLength() const override { return LENGTE; }
    double getDefaultMaxSnelheid() const override { return MAX_SNELHEID; }
    double getDefaultMaxVersnelling() const override { return MAX_VERSNELLING; }
    double getDefaultMaxRemfactor() const override { return MAX_REMFACTOR; }
    double getDefaultMinVolgafstand() const override { return MIN_VOLGAFSTAND; }

public:
    using Voertuig::Voertuig;
};

class Bus : public Voertuig {
protected:
    static constexpr double LENGTE = 12.0;
    static constexpr double MAX_SNELHEID = 11.4;
    static constexpr double MAX_VERSNELLING = 1.22;
    static constexpr double MAX_REMFACTOR = 4.29;
    static constexpr double MIN_VOLGAFSTAND = 12.0;

    double getDefaultLength() const override { return LENGTE; }
    double getDefaultMaxSnelheid() const override { return MAX_SNELHEID; }
    double getDefaultMaxVersnelling() const override { return MAX_VERSNELLING; }
    double getDefaultMaxRemfactor() const override { return MAX_REMFACTOR; }
    double getDefaultMinVolgafstand() const override { return MIN_VOLGAFSTAND; }

public:
    using Voertuig::Voertuig;
};

class Brandweerwagen : public Voertuig {
protected:
    static constexpr double LENGTE = 10.0;
    static constexpr double MAX_SNELHEID = 14.6;
    static constexpr double MAX_VERSNELLING = 1.33;
    static constexpr double MAX_REMFACTOR = 4.56;
    static constexpr double MIN_VOLGAFSTAND = 10.0;

    double getDefaultLength() const override { return LENGTE; }
    double getDefaultMaxSnelheid() const override { return MAX_SNELHEID; }
    double getDefaultMaxVersnelling() const override { return MAX_VERSNELLING; }
    double getDefaultMaxRemfactor() const override { return MAX_REMFACTOR; }
    double getDefaultMinVolgafstand() const override { return MIN_VOLGAFSTAND; }

public:
    using Voertuig::Voertuig;
};

class Ziekenwagen : public Voertuig {
protected:
    static constexpr double LENGTE = 8.0;
    static constexpr double MAX_SNELHEID = 15.5;
    static constexpr double MAX_VERSNELLING = 1.44;
    static constexpr double MAX_REMFACTOR = 4.47;
    static constexpr double MIN_VOLGAFSTAND = 8.0;

    double getDefaultLength() const override { return LENGTE; }
    double getDefaultMaxSnelheid() const override { return MAX_SNELHEID; }
    double getDefaultMaxVersnelling() const override { return MAX_VERSNELLING; }
    double getDefaultMaxRemfactor() const override { return MAX_REMFACTOR; }
    double getDefaultMinVolgafstand() const override { return MIN_VOLGAFSTAND; }

public:
    using Voertuig::Voertuig;
};

class Politiecombi : public Voertuig {
protected:
    static constexpr double LENGTE = 6.0;
    static constexpr double MAX_SNELHEID = 17.2;
    static constexpr double MAX_VERSNELLING = 1.55;
    static constexpr double MAX_REMFACTOR = 4.92;
    static constexpr double MIN_VOLGAFSTAND = 6.0;

    double getDefaultLength() const override { return LENGTE; }
    double getDefaultMaxSnelheid() const override { return MAX_SNELHEID; }
    double getDefaultMaxVersnelling() const override { return MAX_VERSNELLING; }
    double getDefaultMaxRemfactor() const override { return MAX_REMFACTOR; }
    double getDefaultMinVolgafstand() const override { return MIN_VOLGAFSTAND; }

public:
    using Voertuig::Voertuig;
};

#endif // VOERTUIG_H
 */
