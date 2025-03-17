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
