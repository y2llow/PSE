#include "TypeVoertuig.h"

TypeVoertuig TypeVoertuig::createVoertuigType(VoertuigType type) {
    switch (type) {
        case VoertuigType::AUTO:
            return TypeVoertuig(4, 16.6, 1.44, 4.61, 4);

        case VoertuigType::BUS:
            return TypeVoertuig(12, 11.4, 1.22, 4.29, 12);

        case VoertuigType::BRANDWEERWAGEN:
            return TypeVoertuig(10, 14.6, 1.33, 4.56, 10);

        case VoertuigType::ZIEKENWAGEN:
            return TypeVoertuig(8, 15.5, 1.44, 4.47, 8);

        case VoertuigType::POLITIECOMBI:
            return TypeVoertuig(6, 17.2, 1.55, 4.92, 6);

        default:
            // Standaard terugvallen op AUTO als er een onbekend type is
            return TypeVoertuig(4, 16.6, 1.44, 4.61, 4);
    }
}

double TypeVoertuig::getLengte() const {return lengte;}
double TypeVoertuig::getMaxSnelheid() const { return maxSnelheid; }
double TypeVoertuig::getMaxVersnelling() const { return maxVersnelling; }
double TypeVoertuig::getMaxRemfactor() const { return maxRemfactor; }
double TypeVoertuig::getMinVolgafstand() const { return minVolgafstand; }
