//
// Created by s0246031@ad.ua.ac.be on 3/17/25.
//

#ifndef SIMPRINTER_H
#define SIMPRINTER_H


#include "../elementen/Voertuig.h"

class SimPrinter {
public:
    static void printStatus(Voertuig const* vehicle, double status);
};



#endif //SIMPRINTER_H
