#ifndef VOERTUIG_H
#define VOERTUIG_H
#include <string>
#include <vector>

#include "Baan.h"
#include "TypeVoertuig.h"

using namespace std;

class Voertuig {
    // Hier staan gewoon de standaard waarden dat elke voertuig zal hebben
    Baan *baan;
    double positie;
    double snelheid;
    int id;
    double versnelling = 0;
    double kvmax = 0;

    // Nieuw veld voor het type voertuig
    TypeVoertuig typeVoertuig;
    VoertuigType type;

public:
    Voertuig() = default;

    /*
     zo kan kan je een oject aanlmaken
        Voertuig auto1(baan, positie, VoertuigType::AUTO);
        Voertuig bus1(baan, positie, VoertuigType::BUS);
    */

    // Constructor met het type voertuig
    Voertuig(Baan *baan, const int positie, VoertuigType type = VoertuigType::AUTO)
        : baan(baan),
          positie(positie),
          type(type) {
        // Initialiseer het typeVoertuig op basis van het gekozen type
        typeVoertuig = TypeVoertuig::createVoertuigType(type);
    }

    Baan *getBaan() const;

    void setBaan(Baan *weg);

    double getPositie() const;

    void setPositie(double positie);

    double getSnelheid() const;

    void setSnelheid(double snelheid);

    int getId() const;

    void setId(int id);

    double getVersnelling() const;

    void setVersnelling(double versnelling);

    // Nieuwe methode om de lengte van het voertuig op te halen van het typeVoertuig object
    double getLength() const {
        return typeVoertuig.getLengte();
    }

    // Andere voertuigspecifieke parameters ophalen
    double getMaxSnelheid() const {
        return typeVoertuig.getMaxSnelheid();
    }

    double getMaxVersnelling() const {
        return typeVoertuig.getMaxVersnelling();
    }

    double getMaxRemfactor() const {
        return typeVoertuig.getMaxRemfactor();
    }

    double getMinVolgafstand() const {
        return typeVoertuig.getMinVolgafstand();
    }

    VoertuigType getType() const {
        return type;
    }

    double getKvmax() const;

    void setKvmax(double kvmax);

    void UpdateVersnellingVoorStoppen();

    void simulateStep();

    bool getPrioriteit() {
        return typeVoertuig.getPrioriteit();
    }
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
