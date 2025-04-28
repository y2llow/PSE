#ifndef TYPEVOERTUIG_H
#define TYPEVOERTUIG_H

enum class VoertuigType {
    AUTO,
    BUS,
    BRANDWEERWAGEN,
    ZIEKENWAGEN,
    POLITIECOMBI
};

class TypeVoertuig {
public:
    TypeVoertuig() = default;

    // Constructor met standaardwaarden
    TypeVoertuig(double lengte, double maxSnelheid, double maxVersnelling,
                 double maxRemfactor, double minVolgafstand, bool prioriteitsvoertuig)
            : lengte(lengte),
              maxSnelheid(maxSnelheid),
              maxVersnelling(maxVersnelling),
              maxRemfactor(maxRemfactor),
              minVolgafstand(minVolgafstand),
              prioriteitsVoertuig(prioriteitsvoertuig){}

    // Getters voor de constante waarden
    double getLengte() const;
    double getMaxSnelheid() const;
    double getMaxVersnelling() const;
    double getMaxRemfactor() const;
    double getMinVolgafstand() const;
    bool getPrioriteit() const;

    // Fabrieks-functie om het juiste type voertuig te maken
    static TypeVoertuig createVoertuigType(VoertuigType type);

private:
    double lengte;
    double maxSnelheid;
    double maxVersnelling;
    double maxRemfactor;
    double minVolgafstand;
    bool prioriteitsVoertuig;
};

#endif // TYPEVOERTUIG_H