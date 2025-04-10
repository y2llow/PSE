//
// Created by s0246031@ad.ua.ac.be on 3/17/25.
//

#include "SimPrinter.h"

#include <iostream>

using namespace std;


void SimPrinter::printStatus(Voertuig const* voertuig, double status) {
    cout << "Tijd: " << status << endl;

    cout << "Voertuig " << voertuig->getId() << "\n"
        << "-> baan: " << voertuig->getBaan() << "\n"
        << "-> positie: " << voertuig->getPositie() << "\n"<<endl;
}
/*
 * //
// Created by s0246031@ad.ua.ac.be on 3/17/25.
//

#include "SimPrinter.h"

#include <iostream>

using namespace std;


void SimPrinter::printStatus(Voertuig const* voertuig, double status) {
    cout << "Tijd: " << status << endl;

    // Bepaal het type voertuig als string
    string typeStr;
    switch(voertuig->getType()) {
        case VoertuigType::AUTO: typeStr = "Auto"; break;
        case VoertuigType::BUS: typeStr = "Bus"; break;
        case VoertuigType::BRANDWEERWAGEN: typeStr = "Brandweerwagen"; break;
        case VoertuigType::ZIEKENWAGEN: typeStr = "Ziekenwagen"; break;
        case VoertuigType::POLITIECOMBI: typeStr = "Politiecombi"; break;
        default: typeStr = "Onbekend"; break;
    }

    cout << "Voertuig " << voertuig->getId() << " (" << typeStr << ")\n"
         << "-> baan: " << voertuig->getBaan()->getNaam() << "\n"  // Gebruik getNaam() voor de baan
         << "-> positie: " << voertuig->getPositie() << "\n"
         << "-> snelheid: " << voertuig->getSnelheid() << "\n"
         << "-> lengte: " << voertuig->getLength() << "\n"<<endl;
}
 */